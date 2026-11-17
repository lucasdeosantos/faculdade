import { normalizeNames } from './normalizeNames.js';

export class JSONParser {
  parse (texto) {
    const cidades = JSON.parse(texto);
    if (Array.isArray(cidades)) return normalizeNames(cidades, 'Nome');
    if (cidades && Array.isArray(cidades.estados)) {
        return cidades.estados.flatMap(estado =>
        estado.cidades.map(cidade => ({ name: cidade }))
        );
    }
    throw new Error('JSON: estrutura de cidades inválida.');
  }
}
