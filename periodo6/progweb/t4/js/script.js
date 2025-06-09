let studentsData = {};
let currentStudent = null;
let allDisciplineCodes = new Set();

$(document).ready(function () {
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

    $('#searchButton').on('click', function () {
        const ra = $('#raInput').val().trim();
        if (ra)
            displayStudentCurriculum(ra);
        else
            alert("Por favor, digite o RA do aluno.");
    });

    $('#raInput').on('keypress', function (e) {
        if (e.which === 13) {
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
    allDisciplineCodes = new Set();

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
                disciplinas: {}
            };
        }

        if (!studentsData[ra].disciplinas[codigoDisciplina]) {
            studentsData[ra].disciplinas[codigoDisciplina] = {
                nome: nomeDisciplina,
                historico: [],
                ultimaMatricula: null,
                status: 'Não Cursado'
            };
        }

        studentsData[ra].disciplinas[codigoDisciplina].historico.push({
            ano,
            semestre: periodo,
            nota,
            frequencia,
            situacao
        });
    });

    for (const ra in studentsData) {
        const student = studentsData[ra];
        for (const discCode in student.disciplinas) {
            const discipline = student.disciplinas[discCode];
            discipline.historico.sort(compareSemesters);

            if (discipline.historico.length > 0) {
                discipline.ultimaMatricula = discipline.historico.at(-1);
                discipline.status = discipline.ultimaMatricula.situacao;
            }
        }
    }
}

function displayStudentCurriculum(ra) {
    const student = studentsData[ra];

    if (!student) {
        $('#curriculumGrid').html(`<p class="placeholder-text error">Aluno com RA "${ra}" não encontrado.</p>`);
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

    const curriculumLayout = {};

    for (const discCode of allDisciplineCodes) {
        const discipline = student.disciplinas[discCode];
        let nome = `(Sem Dados) ${discCode}`;
        let statusClass = 'status-nao-cursado';
        let semestre = 'Não Cursado';

        if (discipline) {
            nome = discipline.nome;

            const statusText = discipline.status.toLowerCase();

            if (/aprovado/i.test(statusText))
                statusClass = 'status-aprovado';
            else if (/reprovado/i.test(statusText))
                statusClass = 'status-reprovado';
            else if (/matriculado/i.test(statusText))
                statusClass = 'status-matriculado';
            else if (/equival[êe]ncia/i.test(statusText))
                statusClass = 'status-equivalencia';
            else
                statusClass = 'status-nao-cursado';

            if (discipline.ultimaMatricula?.semestre)
                semestre = discipline.ultimaMatricula.semestre;
            else if (discipline.historico.length > 0)
                semestre = discipline.historico[0].semestre;
            else
                semestre = 'Semestre Não Definido';
        }

        if (!curriculumLayout[semestre]) curriculumLayout[semestre] = [];

        curriculumLayout[semestre].push({
            code: discCode,
            name: nome,
            statusClass,
            displayName: nome
        });
    }

    const sortedSemesters = Object.keys(curriculumLayout).sort((a, b) => {
        const regex = /(\d{4})\s*\/?\s*(\d+o\.)?/;

        const matchA = a.match(regex);
        const matchB = b.match(regex);

        if (matchA && matchB) {
            const yearA = parseInt(matchA[1]);
            const yearB = parseInt(matchB[1]);
            if (yearA !== yearB) return yearA - yearB;

            const semA = matchA[2]?.includes("1o.") ? 1 : 2;
            const semB = matchB[2]?.includes("1o.") ? 1 : 2;
            return semA - semB;
        }

        if (a === 'Não Cursado') return 1;
        if (b === 'Não Cursado') return -1;
        if (a === 'Semestre Não Definido') return 1;
        if (b === 'Semestre Não Definido') return -1;

        return a.localeCompare(b);
    });

    for (const sem of sortedSemesters) {
        const $semesterDiv = $('<div class="semester"></div>');
        $semesterDiv.append(`<h3>${sem}</h3>`);
        const $disciplinesContainer = $('<div class="semester-disciplines"></div>');

        curriculumLayout[sem].sort((a, b) => a.code.localeCompare(b.code));

        curriculumLayout[sem].forEach(({ code, name, statusClass, displayName }) => {
            const $disciplineBox = $(`
                <div class="discipline-box ${statusClass}"
                     data-discipline-code="${code}"
                     data-discipline-name="${name}">
                    <strong>${code}</strong>
                    <span>${displayName}</span>
                </div>
            `);

            $disciplineBox.on('click', () => showDisciplineDetails(code));
            $disciplineBox.on('contextmenu', (e) => {
                e.preventDefault();
                showDisciplineHistory(code);
            });

            $disciplinesContainer.append($disciplineBox);
        });

        $semesterDiv.append($disciplinesContainer);
        $('#curriculumGrid').append($semesterDiv);
    }
}

function showDisciplineDetails(code) {
    if (!currentStudent) return;
    const discipline = currentStudent.disciplinas[code];

    $('#popupDisciplinaNome').text(`${code} / ${discipline?.nome ?? '(Desconhecido)'}`);
    $('#popupUltimaVezCursada').text(discipline?.ultimaMatricula ? `${discipline.ultimaMatricula.ano}/${discipline.ultimaMatricula.semestre}` : 'N/A');
    $('#popupNota').text(discipline?.ultimaMatricula?.nota ?? 'N/A');
    $('#popupFrequencia').text(discipline?.ultimaMatricula?.frequencia ?? 'N/A');

    $('#disciplineDetailsPopup').show();
}

function showDisciplineHistory(code) {
    if (!currentStudent) return;
    const discipline = currentStudent.disciplinas[code];
    const $content = $('#popupHistoryContent');
    $content.empty();

    if (discipline?.historico?.length > 0) {
        let html = `
            <h3>${code} - ${discipline.nome}</h3>
            <table>
                <thead><tr>
                    <th>Ano</th><th>Semestre</th><th>Nota</th><th>Frequência</th><th>Situação</th>
                </tr></thead>
                <tbody>
        `;

        const sortedHistory = [...discipline.historico].sort(compareSemesters);
        sortedHistory.forEach(({ ano, semestre, nota, frequencia, situacao }) => {
            html += `<tr><td>${ano}</td><td>${semestre}</td><td>${nota}</td><td>${frequencia}</td><td>${situacao}</td></tr>`;
        });

        html += '</tbody></table>';
        $content.append(html);
    } 
    else {
        $content.append(`<p>Não há histórico para a disciplina ${code} - ${discipline?.nome ?? ''}.</p>`);
    }

    $('#disciplineHistoryPopup').show();
}
