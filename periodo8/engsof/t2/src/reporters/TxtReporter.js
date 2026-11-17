export class TxtReporter {
  reportar(cidades) {
    let result = `Relatório de Nomes de Cidades\n=============================\n`;

    for (let i = 0; i < cidades.length; i++)
      result += '* ' + cidades[i]['name'] + '\n';

    return result;
  }
}
