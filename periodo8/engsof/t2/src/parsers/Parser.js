import { normalizarNomes } from './normalizarNomes.js';

export class Parser {
  parse(texto) {
    const { cidades, key } = this.lerCidades(texto);
    return key === undefined ? cidades : normalizarNomes(cidades, key);
  }

  lerCidades(texto) {
    throw new Error('O parser deve implementar lerCidades(texto).');
  }
}
