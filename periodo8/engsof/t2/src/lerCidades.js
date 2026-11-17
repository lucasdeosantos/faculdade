import { readFileSync } from 'node:fs';

export function lerCidades(caminho, parser) {
  const texto = readFileSync(caminho, 'utf8').replace(/^\uFEFF/, '');
  return parser.parse(texto);
}
