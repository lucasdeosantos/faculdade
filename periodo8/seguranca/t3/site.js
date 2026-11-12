(function () {
  // Laboratórios: um em modo vulnerável (ataque) e outro protegido (defesa)
  document.querySelectorAll('.lab').forEach((el) => createLab(el, el.dataset.modo));

  // Barra de progresso
  const barra = document.getElementById('progresso');
  function progresso() {
    const max = document.documentElement.scrollHeight - innerHeight;
    barra.style.width = (max > 0 ? (scrollY / max) * 100 : 0) + '%';
  }
  addEventListener('scroll', progresso, { passive: true });
  progresso();

  // Menu: destaca a seção em foco
  const links = Array.from(document.querySelectorAll('.menu a'));
  const secoes = links.map((a) => document.querySelector(a.getAttribute('href')));
  function marcarMenu() {
    let atual = -1;
    secoes.forEach((s, i) => { if (s && s.getBoundingClientRect().top <= innerHeight * 0.35) atual = i; });
    links.forEach((a, i) => a.classList.toggle('ativo', i === atual));
  }
  addEventListener('scroll', marcarMenu, { passive: true });
  marcarMenu();

  // Aparecer ao rolar
  const itens = document.querySelectorAll('.rev');
  if ('IntersectionObserver' in window) {
    const io = new IntersectionObserver((es) => es.forEach((e) => {
      if (e.isIntersecting) { e.target.classList.add('in'); io.unobserve(e.target); }
    }), { threshold: 0.12 });
    itens.forEach((i) => io.observe(i));
  } else {
    itens.forEach((i) => i.classList.add('in'));
  }
})();
