# XSS (Cross-Site Scripting): site da aula + laboratório

Site de página única: basta rolar de cima para baixo. HTML, CSS e JavaScript puros, funciona offline.
Uso exclusivamente educacional, com dados fictícios.

## Como executar
```
unzip xss-site.zip
cd xss-site
abra o index.html no navegador (duplo clique)
```

## Estrutura
```
index.html  → conteúdo (seções de cima para baixo)
site.css    → visual (paleta e fontes do slide original)
site.js     → menu fixo, barra de progresso, animação ao rolar
lab.js      → laboratório (procure "FALHA" e "CORREÇÃO" na função shopRuntime)
fonts/      → fontes locais (sem internet)
```

## Ordem da página (segue a aula)
1. O que é XSS, contexto e ambiente, como funciona, três tipos
2. Ataque: passo a passo, infraestrutura e ferramentas, **demo vulnerável**, impactos
3. Defesas: codificar, sanitizar, CSP, cookies, camadas
4. **Demo protegida** (o mesmo ataque falha), conclusão e referências

## Laboratório
- A "loja" roda num `<iframe sandbox>`, isolada do resto da página.
- Cenários: Refletido (busca), Armazenado (comentários), DOM (fragmento da URL).
- **Vulnerável / Protegido** liga ou desliga as 3 defesas; os chips ligam cada uma separadamente:
  codificar/sanitizar, CSP (real, via `<meta>`) e cookie HttpOnly (simulado).
- Trocar o modo reexecuta o mesmo ataque com o resultado novo.
- Limitação: não há servidor; cookie e comentários salvos são simulados em JS.

## Roteiro de 5 minutos
| Tempo | Onde | O que fazer |
|---|---|---|
| 0:00–1:00 | O que é / Como funciona | Dado do usuário vira código. Citar os 3 tipos. |
| 1:00–2:30 | Ataque ao vivo (modo Vulnerável) | "Uso normal", depois **alert()**, mostrar a faixa **FALHA**. Clicar **Roubar sessão** e ler o Registro. |
| 2:30–3:00 | Ataque ao vivo | Aba **Armazenado**: publicar o payload e "Recarregar (vítima)". |
| 3:00–4:00 | Defesas | Codificar, sanitizar, CSP, HttpOnly: defesa em camadas. |
| 4:00–5:00 | Defesa ao vivo (modo Protegido) | Repetir o **mesmo** alert(): vira texto. Ligar só CSP, depois só HttpOnly. Regra final: nunca confie na entrada. |
