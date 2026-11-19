import { Reporter } from './Reporter.js';

export class TxtReporter extends Reporter {
  reportar(cidades) {
    let result = `Relatório de Nomes de Cidades\n=============================\n`;

    for (let i = 0; i < cidades.length; i++) {
      result += '* ' + cidades[i]['name'] + '\n';
    }

    return result;
  }
}
