(function () {
  const barra = document.getElementById('progresso');
  function progresso() {
    const max = document.documentElement.scrollHeight - innerHeight;
    barra.style.width = (max > 0 ? (scrollY / max) * 100 : 0) + '%';
  }
  addEventListener('scroll', progresso, { passive: true });
  addEventListener('resize', progresso);
  progresso();

  const links = Array.from(document.querySelectorAll('.menu a'));
  const secoes = links.map((a) => document.querySelector(a.getAttribute('href')));
  function marcarMenu() {
    let atual = -1;
    secoes.forEach((s, i) => { if (s && s.getBoundingClientRect().top <= innerHeight * 0.35) atual = i; });
    links.forEach((a, i) => {
      a.classList.toggle('ativo', i === atual);
      if (i === atual) a.setAttribute('aria-current', 'location');
      else a.removeAttribute('aria-current');
    });
  }
  addEventListener('scroll', marcarMenu, { passive: true });
  addEventListener('resize', marcarMenu);
  marcarMenu();

  const itens = document.querySelectorAll('.rev');
  if ('IntersectionObserver' in window) {
    const io = new IntersectionObserver((es) => es.forEach((e) => {
      if (e.isIntersecting) { e.target.classList.add('in'); io.unobserve(e.target); }
    }), { threshold: 0.12 });
    itens.forEach((i) => io.observe(i));
    document.documentElement.classList.add('js');
  }
})();
