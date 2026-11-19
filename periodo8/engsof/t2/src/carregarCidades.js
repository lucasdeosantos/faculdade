import { readFileSync } from 'node:fs';

const errosDeLeitura = new Map([
  ['ENOENT', 'Arquivo não encontrado'],
  ['ENOTDIR', 'Um componente do caminho não é um diretório'],
  ['EISDIR', 'O caminho aponta para um diretório'],
  ['EACCES', 'Permissão negada'],
  ['EPERM', 'Permissão negada'],
]);

export function carregarCidades(caminho, parser) {
  let texto;
  try {
    texto = readFileSync(caminho, 'utf8').replace(/^\uFEFF/, '');
  } catch (error) {
    const motivo = errosDeLeitura.get(error.code) ?? 'Falha na leitura (' + (error.code ?? error.message) + ')';
    throw new Error(motivo + ': "' + caminho + '".', { cause: error });
  }
  
  try {
    return parser.parse(texto);
  } catch (error) {
    const motivo = error instanceof SyntaxError ? 'Sintaxe inválida. ' + error.message : error.message;
    throw new Error('Não foi possível interpretar "' + caminho + '". ' + motivo, { cause: error });
  }
}
