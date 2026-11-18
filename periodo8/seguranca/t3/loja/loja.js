function mostrarBusca() {
  const termo = new URLSearchParams(location.search).get('q') || '';
  const resultado = document.querySelector('#resultado');
  document.querySelector('#q').value = termo;
  resultado.hidden = !termo;

  // VULNERABILIDADE: innerHTML interpreta o parâmetro ?q= como HTML.
  resultado.innerHTML = 'Você buscou: ' + termo;
  // DEFESA: textContent exibe a entrada como texto e impede que o navegador interprete o HTML injetado.
  // resultado.textContent = 'Você buscou: ' + termo;

  // DOMPurify: sanitiza a busca antes de renderizar o HTML permitido.
  // resultado.innerHTML = DOMPurify.sanitize('Você buscou: ' + termo);

  const filtro = termo.trim().toLowerCase();
  let encontrou = false;
  for (const produto of document.querySelectorAll('.produto')) {
    produto.hidden = !produto.querySelector('h2').textContent.toLowerCase().includes(filtro);
    if (!produto.hidden) encontrou = true;
  }
  document.querySelector('#sem-produtos').hidden = encontrou;
}

const chaveSessao = 'loja.sessao';
const usuarioDemo = { usuario: 'joaozinho', userId: '123456789' };
const perfilDemo = document.querySelector('#perfil-demo');
const iniciarSessao = document.querySelector('#iniciar-sessao');

try {
  const sessao = JSON.parse(sessionStorage.getItem(chaveSessao));
  if (sessao) {
    mostrarPerfilDemo(sessao);
    if (sessao.token && !sessao.sessionId) {
      sessionStorage.setItem(chaveSessao, JSON.stringify({ ...usuarioDemo, sessionId: sessao.token }));
    }
  }
} catch {}

function mostrarPerfilDemo(sessao) {
  document.querySelector('#nome-perfil').textContent = sessao.usuario;
  document.querySelector('#aviso-perfil').textContent = '';
  perfilDemo.hidden = false;
  iniciarSessao.hidden = true;
}

iniciarSessao.addEventListener('click', () => {
  const id = crypto.getRandomValues(new Uint32Array(1))[0].toString(16);
  const sessao = { ...usuarioDemo, sessionId: 'jd-demo-' + id };
  try {
    sessionStorage.setItem(chaveSessao, JSON.stringify(sessao));
    mostrarPerfilDemo(sessao);
  } catch {
    document.querySelector('#aviso-perfil').textContent = 'O navegador bloqueou o armazenamento da sessão de teste.';
  }
});

mostrarBusca();

document.querySelector('#restaurar').addEventListener('click', () => {
  try {
    sessionStorage.removeItem(chaveSessao);
  } catch {}
  location.replace(new URL('./index.html', location.href).href);
});
