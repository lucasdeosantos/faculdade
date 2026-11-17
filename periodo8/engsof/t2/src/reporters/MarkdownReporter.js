export class MarkdownReporter {
  reportar(cidades) {
    let result = `# Relatório de Nomes de Cidades\n\n`;

    for (const { name } of cidades) {
      const texto = name.replaceAll('&', '&amp;').replaceAll('\r', '&#13;').replaceAll('\n', '&#10;');
      result += '- ' + texto + '\n';
    }

    return result;
  }
}
