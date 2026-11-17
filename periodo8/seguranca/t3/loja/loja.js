const chaveAvaliacoes = 'xss-loja-estatica.avaliacoes';
const lista = document.querySelector('#lista-avaliacoes');
const aviso = document.querySelector('#aviso');

function mostrarBusca() {
  const termo = new URLSearchParams(location.search).get('q') || '';
  const resultado = document.querySelector('#resultado');
  document.querySelector('#q').value = termo;
  resultado.hidden = !termo;

  // 1. Busca — FALHA: innerHTML interpreta o parâmetro ?q= como HTML.
  // DEFESA: substitua a linha ativa por esta para exibir somente texto:
  // resultado.textContent = 'Você buscou: ' + termo;
  resultado.innerHTML = 'Você buscou: ' + termo;

  const filtro = termo.trim().toLowerCase();
  let encontrou = false;
  for (const produto of document.querySelectorAll('.produto')) {
    produto.hidden = !produto.querySelector('h2').textContent.toLowerCase().includes(filtro);
    if (!produto.hidden) encontrou = true;
  }
  document.querySelector('#sem-produtos').hidden = encontrou;
}

function adicionarAvaliacao(texto) {
  const item = document.createElement('li');

  // 2. Avaliações — FALHA: innerHTML interpreta uma avaliação já salva.
  // DEFESA: substitua a linha ativa pelo trecho abaixo. Primeiro, transforme
  // a entrada em texto. Depois, reabilite somente as tags literais <b> e </b>.
  // item.textContent = texto;
  // item.innerHTML = item.innerHTML
  //   .replaceAll('&lt;b&gt;', '<b>')
  //   .replaceAll('&lt;/b&gt;', '</b>');
  item.innerHTML = texto;

  lista.append(item);
}

function mostrarSaudacao() {
  const nome = new URLSearchParams(location.hash.slice(1)).get('nome') || 'Visitante';
  const saudacao = document.querySelector('#saudacao');

  // 3. Conta — FALHA: innerHTML interpreta o nome em #nome= como HTML.
  // DEFESA: substitua a linha ativa por esta para exibir somente texto:
  // saudacao.textContent = 'Olá, ' + nome;
  saudacao.innerHTML = 'Olá, ' + nome;
}

function mostrarAviso(mensagem) {
  aviso.textContent = mensagem;
  aviso.hidden = false;
}

function lerAvaliacoes() {
  try {
    const salvas = JSON.parse(localStorage.getItem(chaveAvaliacoes) || '[]');
    if (Array.isArray(salvas) && salvas.every(texto => typeof texto === 'string')) return salvas;
  } catch {
    // A loja ainda pode mostrar o catálogo e aceitar avaliações temporárias.
  }
  mostrarAviso('Não foi possível ler as avaliações salvas.');
  return [];
}

const avaliacoes = lerAvaliacoes();
adicionarAvaliacao('Gostei do espaço e dos bolsos da mochila.');
avaliacoes.forEach(adicionarAvaliacao);

document.querySelector('#avaliar').addEventListener('submit', evento => {
  evento.preventDefault();
  const campo = document.querySelector('#comentario');
  const texto = campo.value.trim();
  if (!texto) return;

  avaliacoes.push(texto);
  try {
    localStorage.setItem(chaveAvaliacoes, JSON.stringify(avaliacoes));
    aviso.hidden = true;
  } catch {
    mostrarAviso('A avaliação fica visível até recarregar a página.');
  }
  adicionarAvaliacao(texto);
  campo.value = '';
});

mostrarBusca();
mostrarSaudacao();
addEventListener('hashchange', mostrarSaudacao);

// Restaurar: limpa as avaliações salvas e tira ?q=/#nome= da URL, para não
// repetir o mesmo payload assim que a página recarregar.
document.querySelector('#restaurar').addEventListener('click', () => {
  try {
    localStorage.removeItem(chaveAvaliacoes);
  } catch {
    // Limpa a URL mesmo quando o navegador bloqueia o armazenamento.
  }
  location.replace(new URL('./index.html', location.href).href);
});
