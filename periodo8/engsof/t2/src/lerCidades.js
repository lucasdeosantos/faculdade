import { readFileSync } from 'node:fs';

export function lerCidades(caminho, parse) {
  const texto = readFileSync(caminho, 'utf8').replace(/^\uFEFF/, '');
  return parse(texto);
}
