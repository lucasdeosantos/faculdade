let studentsData = {};
let currentStudent = null;
let allDisciplineCodes = new Set();
let curriculumGradeData = null;
let disciplineNamesMap = null;

$(document).ready(function () {
    $.ajax({
        type: "GET",
        url: "data/grade.json",
        dataType: "json",
        success: function (json) {
            curriculumGradeData = json.curriculum; // A estrutura semestral
            disciplineNamesMap = json.disciplines; // O mapa de códigos para nomes
            console.log("Curriculum Grade data loaded successfully:", curriculumGradeData);
            console.log("Discipline Names Map loaded successfully:", disciplineNamesMap);

            $.ajax({
                type: "GET",
                url: "data/alunos.xml",
                dataType: "xml",
                success: function (xml) {
                    parseAlunosXML(xml);
                    console.log("XML data loaded and parsed successfully.");
                    console.log("Parsed Students Data:", studentsData);
                    console.log("All Unique Discipline Codes:", Array.from(allDisciplineCodes));
                },
                error: function (xhr, status, error) {
                    console.error("Error loading XML file:", status, error);
                    alert("Erro ao carregar os dados dos alunos. Verifique o console para mais detalhes.");
                }
            });

        },
        error: function (xhr, status, error) {
            console.error("Error loading curriculum grade JSON file:", status, error);
            alert("Erro ao carregar os dados da grade curricular. Verifique o console para mais detalhes.");
        }
    });


    $('#searchButton').on('click', function () {
        const ra = $('#raInput').val().trim();
        if (ra)
            displayStudentCurriculum(ra);
        else
            alert("Por favor, digite o RA do aluno.");
    });

    $('#raInput').on('keypress', function (e) {
        if (e.which === 13) { // Tecla Enter
            $('#searchButton').click();
        }
    });

    $('.close-button').on('click', function () {
        const modalId = $(this).data('modal');
        $('#' + modalId).hide();
    });

    $(window).on('click', function (event) {
        if ($(event.target).is('.modal')) {
            $(event.target).hide();
        }
    });
});

function loadCurriculumGrade() {
    $.ajax({
        type: "GET",
        url: "data/grade.json",
        dataType: "json",
        success: function (json) {
            curriculumGradeData = json.curriculum; // A estrutura semestral
            disciplineNamesMap = json.disciplines; // O mapa de códigos para nomes
            console.log("Curriculum Grade data loaded successfully:", curriculumGradeData);
            console.log("Discipline Names Map loaded successfully:", disciplineNamesMap);
        },
        error: function (xhr, status, error) {
            console.error("Error loading curriculum grade JSON file:", status, error);
            alert("Erro ao carregar os dados da grade curricular. Verifique o console para mais detalhes.");
        }
    });
}

function compareSemesters(a, b) {
    const yearA = parseInt(a.ano);
    const yearB = parseInt(b.ano);
    const periodA = a.semestre.toLowerCase();
    const periodB = b.semestre.toLowerCase();

    if (yearA !== yearB) return yearA - yearB;

    if (periodA.includes('1o.') && periodB.includes('2o.')) return -1;
    if (periodA.includes('2o.') && periodB.includes('1o.')) return 1;

    return periodA.localeCompare(periodB);
}

function parseAlunosXML(xml) {
    studentsData = {};
    allDisciplineCodes = new Set(); // Resetar para garantir que apenas disciplinas do XML são consideradas para allDisciplineCodes

    $(xml).find('ALUNO').each(function () {
        const $aluno = $(this);

        const ra = $aluno.find('MATR_ALUNO').text().trim();
        const nomeAluno = $aluno.find('NOME_ALUNO').text().trim();
        const nomeCurso = $aluno.find('NOME_CURSO').text().trim();
        const codigoDisciplina = $aluno.find('COD_ATIV_CURRIC').text().trim();
        const nomeDisciplina = $aluno.find('NOME_ATIV_CURRIC').text().trim();
        const ano = $aluno.find('ANO').text().trim();
        const periodo = $aluno.find('PERIODO').text().trim();
        const nota = $aluno.find('MEDIA_FINAL').text().trim();
        const frequencia = $aluno.find('FREQUENCIA').text().trim();
        const situacao = $aluno.find('SITUACAO').text().trim();

        allDisciplineCodes.add(codigoDisciplina);

        if (!studentsData[ra]) {
            studentsData[ra] = {
                nome: nomeAluno,
                curso: nomeCurso,
                disciplinas: {} // Objeto para armazenar disciplinas cursadas pelo aluno
            };
        }

        if (!studentsData[ra].disciplinas[codigoDisciplina]) {
            studentsData[ra].disciplinas[codigoDisciplina] = {
                nome: nomeDisciplina,
                historico: [],
                ultimaMatricula: null,
                status: 'Não Cursado', // Estado inicial antes de verificar o histórico
                isOptional: false, // Pode ser útil para identificar se é uma optativa no XML
                isTGI: false, // Pode ser útil para identificar TG I no XML
                isTGII: false // Pode ser útil para identificar TG II no XML
            };

            // Heurística para tentar identificar optativas/TGs pelo nome, se o código não for específico
            const lowerNomeDisc = nomeDisciplina.toLowerCase();
            if (!disciplineNamesMap[codigoDisciplina] || // Se não está no mapa de obrigatórias
                (lowerNomeDisc.includes("tópicos") || lowerNomeDisc.includes("disciplina optativa") || lowerNomeDisc.includes("seminário"))) {
                studentsData[ra].disciplinas[codigoDisciplina].isOptional = true;
            }
            if (lowerNomeDisc.includes("trabalho de graduação") && lowerNomeDisc.includes("i")) {
                studentsData[ra].disciplinas[codigoDisciplina].isTGI = true;
                studentsData[ra].disciplinas[codigoDisciplina].isOptional = false; // Não é apenas optativa, é um TG
            }
            if (lowerNomeDisc.includes("trabalho de graduação") && lowerNomeDisc.includes("ii")) {
                studentsData[ra].disciplinas[codigoDisciplina].isTGII = true;
                studentsData[ra].disciplinas[codigoDisciplina].isOptional = false; // Não é apenas optativa, é um TG
            }
        }

        // Adiciona a matrícula atual ao histórico da disciplina
        studentsData[ra].disciplinas[codigoDisciplina].historico.push({
            ano,
            semestre: periodo,
            nota,
            frequencia,
            situacao
        });
    });

    // Pós-processamento: Determinar o status final e a última matrícula para cada disciplina cursada pelo aluno
    for (const ra in studentsData) {
        const student = studentsData[ra];
        for (const discCode in student.disciplinas) {
            const discipline = student.disciplinas[discCode];
            discipline.historico.sort(compareSemesters); // Garante que o histórico esteja ordenado cronologicamente

            if (discipline.historico.length > 0) {
                discipline.ultimaMatricula = discipline.historico.at(-1); // A última entrada no histórico (a mais recente)
                discipline.status = discipline.ultimaMatricula.situacao;
            }
        }
    }
}

function displayStudentCurriculum(ra) {
    const student = studentsData[ra];

    // Verifica se o aluno foi encontrado
    if (!student) {
        $('#curriculumGrid').html(`<p class="placeholder-text error">Aluno com RA "${ra}" não encontrado.</p>`);
        currentStudent = null;
        return;
    }

    // Verifica se os dados da grade curricular foram carregados
    if (!curriculumGradeData || !disciplineNamesMap) {
        $('#curriculumGrid').html(`<p class="placeholder-text error">Dados da grade curricular não carregados. Por favor, recarregue a página.</p>`);
        currentStudent = null;
        return;
    }

    currentStudent = student;
    $('#curriculumGrid').empty().removeClass('placeholder-text').css({
        display: 'block',
        padding: 0,
        border: 'none',
        backgroundColor: 'transparent'
    });

    // Set para controlar quais códigos de disciplina do histórico do aluno já foram exibidos na grade
    const usedStudentDiscCodesInGrade = new Set();

    // Itera sobre os semestres definidos na grade curricular JSON
    for (const semesterName in curriculumGradeData) {
        const semesterDisciplines = curriculumGradeData[semesterName];

        const $semesterDiv = $('<div class="semester"></div>');
        $semesterDiv.append(`<h3>${semesterName}</h3>`);
        const $disciplinesContainer = $('<div class="semester-disciplines"></div>');

        // Para cada slot de disciplina definida neste semestre na grade JSON
        semesterDisciplines.forEach(gradeDisc => {
            const discCodeInGrade = gradeDisc.code; // Código da grade (ex: CI055, OPT, TGI)
            const discTypeInGrade = gradeDisc.type; // Tipo da disciplina (normal, optativa, tg1, tg2)

            let displayedDiscCode = discCodeInGrade;
            let displayedDiscName = disciplineNamesMap[discCodeInGrade] || `Nome não encontrado para ${discCodeInGrade}`;
            let statusClass = 'status-nao-cursado';
            let actualDisciplineObject = null; // Referência à disciplina real do histórico do aluno

            // Lógica para preencher OPT, TGI, TGII
            if (discTypeInGrade === "optativa" || discTypeInGrade === "tg1" || discTypeInGrade === "tg2") {
                let foundMatch = false;
                // Procura por uma disciplina no histórico do aluno que corresponda ao tipo e ainda não tenha sido usada
                for (const studentDiscCode in student.disciplinas) {
                    const studentDiscipline = student.disciplinas[studentDiscCode];

                    // Pular se já foi usada para preencher outro slot da grade
                    if (usedStudentDiscCodesInGrade.has(studentDiscCode)) {
                        continue;
                    }

                    let matchesType = false;
                    if (discTypeInGrade === "optativa" && studentDiscipline.isOptional) {
                        matchesType = true;
                    } 
                    else if (discTypeInGrade === "tg1" && studentDiscipline.isTGI) {
                        matchesType = true;
                    } 
                    else if (discTypeInGrade === "tg2" && studentDiscipline.isTGII) {
                        matchesType = true;
                    }

                    if (matchesType) {
                        actualDisciplineObject = studentDiscipline;
                        displayedDiscCode = studentDiscCode; // Usar o código da disciplina real
                        displayedDiscName = studentDiscipline.nome; // Usar o nome real da disciplina
                        usedStudentDiscCodesInGrade.add(studentDiscCode); // Marca como usada na grade
                        foundMatch = true;
                        break; // Encontrou uma disciplina para o slot, sai do loop
                    }
                }
                // Se não encontrou nenhuma disciplina real para esse slot de optativa/TG,
                // ele permanece com o nome genérico e status "Não Cursado".
            } 
            else {
                // Para disciplinas normais da grade, verifica se o aluno a cursou
                actualDisciplineObject = student.disciplinas[discCodeInGrade];
                if (actualDisciplineObject) {
                    usedStudentDiscCodesInGrade.add(discCodeInGrade); // Marca como usada na grade
                }
            }

            // Define o status e o nome de exibição com base na disciplina encontrada (ou não)
            if (actualDisciplineObject) {
                const statusText = actualDisciplineObject.status.toLowerCase();

                if (/aprovado/i.test(statusText)) {
                    statusClass = 'status-aprovado';
                } 
                else if (/reprovado/i.test(statusText)) {
                    statusClass = 'status-reprovado';
                } 
                else if (/matriculado/i.test(statusText)) {
                    statusClass = 'status-matriculado';
                } 
                else if (/equival[êe]ncia/i.test(statusText)) {
                    statusClass = 'status-equivalencia';
                } 
                else {
                    statusClass = 'status-nao-cursado'; // Fallback
                }
            }
            else {
                // Se não encontrou nenhuma disciplina real para o slot (principalmente para OPT/TGI/TGII não cursadas)
                // ou se é uma disciplina normal da grade que o aluno não fez
                statusClass = 'status-nao-cursado';
                // O displayedDiscName já está com o nome padrão da grade (ex: "Disciplina Optativa")
            }

            const $disciplineBox = $(`
                <div class="discipline-box ${statusClass}"
                    data-discipline-code="${displayedDiscCode}"
                    data-discipline-name="${displayedDiscName}"
                    data-discipline-type="${discTypeInGrade}">
                    <strong>${displayedDiscCode}</strong>
                    <span>${displayedDiscName}</span>
                </div>
            `);

            // Adiciona eventos de clique para mostrar detalhes (clique normal) e histórico (clique direito)
            // Os popups agora sempre usarão o 'displayedDiscCode' para buscar informações no histórico do aluno
            $disciplineBox.on('click', () => showDisciplineDetails(displayedDiscCode));
            $disciplineBox.on('contextmenu', (e) => {
                e.preventDefault(); // Impede o menu de contexto padrão do navegador
                showDisciplineHistory(displayedDiscCode);
            });

            $disciplinesContainer.append($disciplineBox);
        });

        $semesterDiv.append($disciplinesContainer);
        $('#curriculumGrid').append($semesterDiv);
    }

    // --- Seção "Outras Matérias" ---
    const $otherSubjectsDiv = $('<div class="semester other-subjects"></div>');
    $otherSubjectsDiv.append('<h3>Outras Matérias</h3>');
    const $otherDisciplinesContainer = $('<div class="semester-disciplines"></div>');
    let hasOtherSubjects = false;

    // Itera sobre todas as disciplinas no histórico do aluno
    for (const studentDiscCode in student.disciplinas) {
        // Se a disciplina não foi usada para preencher nenhum slot na grade curricular padrão
        if (!usedStudentDiscCodesInGrade.has(studentDiscCode)) {
            const studentDiscipline = student.disciplinas[studentDiscCode];
            const statusClass = getStatusClass(studentDiscipline.status);
            
            const $disciplineBox = $(`
                <div class="discipline-box ${statusClass}"
                    data-discipline-code="${studentDiscCode}"
                    data-discipline-name="${studentDiscipline.nome}"
                    data-discipline-type="outra">
                    <strong>${studentDiscCode}</strong>
                    <span>${studentDiscipline.nome}</span>
                </div>
            `);

            $disciplineBox.on('click', () => showDisciplineDetails(studentDiscCode));
            $disciplineBox.on('contextmenu', (e) => {
                e.preventDefault();
                showDisciplineHistory(studentDiscCode);
            });

            $otherDisciplinesContainer.append($disciplineBox);
            hasOtherSubjects = true;
        }
    }

    if (hasOtherSubjects) {
        $otherSubjectsDiv.append($otherDisciplinesContainer);
        $('#curriculumGrid').append($otherSubjectsDiv);
    } 
    else {
        $otherSubjectsDiv.append('<p class="placeholder-text">Nenhuma outra matéria encontrada fora da grade curricular.</p>');
        $('#curriculumGrid').append($otherSubjectsDiv);
    }
}

function getStatusClass(statusText) {
    const lowerStatus = statusText.toLowerCase();
    if (/aprovado/i.test(lowerStatus)) {
        return 'status-aprovado';
    } 
    else if (/reprovado/i.test(lowerStatus)) {
        return 'status-reprovado';
    } 
    else if (/matriculado/i.test(lowerStatus)) {
        return 'status-matriculado';
    } 
    else if (/equival[êe]ncia/i.test(lowerStatus)) {
        return 'status-equivalencia';
    }
    return 'status-nao-cursado';
}

function showDisciplineDetails(code) {
    if (!currentStudent) return;
    const discipline = currentStudent.disciplinas[code];

    // Tenta obter o nome da disciplina do histórico do aluno (se disponível para esse código),
    // se não, do mapa de nomes da grade (se for um código "normal"), senão um fallback.
    const disciplineName = discipline?.nome ?? disciplineNamesMap[code] ?? '(Desconhecido)';

    $('#popupDisciplinaNome').text(`${code} / ${disciplineName}`);
    $('#popupUltimaVezCursada').text(discipline?.ultimaMatricula ? `${discipline.ultimaMatricula.ano}/${discipline.ultimaMatricula.semestre}` : 'N/A');
    $('#popupNota').text(discipline?.ultimaMatricula?.nota ?? 'N/A');
    $('#popupFrequencia').text(discipline?.ultimaMatricula?.frequencia ?? 'N/A');

    $('#disciplineDetailsPopup').show();
}

function showDisciplineHistory(code) {
    if (!currentStudent) return;
    const discipline = currentStudent.disciplinas[code];
    const $content = $('#popupHistoryContent');
    $content.empty(); // Limpa o conteúdo anterior

    // Tenta obter o nome da disciplina do histórico do aluno,
    // se não, do mapa de nomes da grade, senão vazio.
    const disciplineName = discipline?.nome ?? disciplineNamesMap[code] ?? '';

    if (discipline?.historico?.length > 0) {
        let html = `
            <h3>${code} - ${disciplineName}</h3>
            <table>
                <thead><tr>
                    <th>Ano</th><th>Semestre</th><th>Nota</th><th>Frequência</th><th>Situação</th>
                </tr></thead>
                <tbody>
        `;

        // O histórico já deve estar ordenado pelo parseAlunosXML, mas uma cópia é feita para segurança
        const sortedHistory = [...discipline.historico].sort(compareSemesters);
        sortedHistory.forEach(({ ano, semestre, nota, frequencia, situacao }) => {
            html += `<tr><td>${ano}</td><td>${semestre}</td><td>${nota}</td><td>${frequencia}</td><td>${situacao}</td></tr>`;
        });

        html += '</tbody></table>';
        $content.append(html);
    } 
    else {
        $content.append(`<p>Não há histórico para a disciplina ${code} - ${disciplineName}.</p>`);
    }

    $('#disciplineHistoryPopup').show();
}