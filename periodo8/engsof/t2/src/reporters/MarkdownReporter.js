import { Reporter } from './Reporter.js';

export class MarkdownReporter extends Reporter {
  reportar(cidades) {
    let result = `# Relatório de Nomes de Cidades\n`;

    for (const { name } of cidades) {
      const texto = name.replaceAll('&', '&amp;').replaceAll('\r', '&#13;').replaceAll('\n', '&#10;');
      result += '- ' + texto + '\n';
    }

    return result;
  }
}
