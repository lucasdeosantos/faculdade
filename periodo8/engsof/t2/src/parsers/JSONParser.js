import { Parser } from './Parser.js';

export class JSONParser extends Parser {
  lerCidades(texto) {
    const cidades = JSON.parse(texto);
    if (Array.isArray(cidades)) return { cidades, key: 'Nome' };
    if (cidades && Array.isArray(cidades.estados)) {
      return {
        cidades: cidades.estados.flatMap((estado, estadoIndex) => {
          if (!Array.isArray(estado?.cidades)) {
            throw new Error('JSON: estado ' + (estadoIndex + 1) + ' deve conter uma lista de cidades.');
          }
          return estado.cidades.map((cidade, cidadeIndex) => {
            if (typeof cidade !== 'string') {
              throw new Error('JSON: cidade ' + (cidadeIndex + 1) + ' do estado ' + (estadoIndex + 1) + ' deve ser texto.');
            }
            return { name: cidade };
          });
        }),
      };
    }
    throw new Error('JSON: estrutura de cidades inválida.');
  }
}
