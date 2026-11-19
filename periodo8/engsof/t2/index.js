import { extname } from 'node:path';
import { carregarCidades } from './src/carregarCidades.js';
import { JSONParser } from './src/parsers/JSONParser.js';
import { CSVParser } from './src/parsers/CSVParser.js';
import { HtmlReporter } from './src/reporters/HtmlReporter.js';
import { TxtReporter } from './src/reporters/TxtReporter.js';
import { MarkdownReporter } from './src/reporters/MarkdownReporter.js';

const help = `Uso: node index.js --source <arquivo.json|arquivo.csv> --format <html|txt|markdown>
     node index.js --help
`;

function parseArguments(args) {
  if (args.length === 1 && args[0] === '--help') return { help: true };
  const options = {};
  for (let i = 0; i < args.length; i += 2) {
    const flag = args[i];
    if (!['--source', '--format'].includes(flag) || !args[i + 1] || args[i + 1].startsWith('-')) {
      throw new Error('Argumentos inválidos. Consulte --help.');
    }
    const key = flag.slice(2);
    if (key in options) throw new Error(`Argumento repetido: ${flag}`);
    options[key] = args[i + 1];
  }
  if (!options.source) throw new Error('Informe --source. Consulte --help.');
  if (!options.format) throw new Error('Informe --format. Consulte --help.');
  return options;
}

const parsers = new Map([['.json', JSONParser], ['.csv', CSVParser]]);
const reporters = new Map([['html', HtmlReporter], ['txt', TxtReporter], ['markdown', MarkdownReporter]]);

function main(args) {
  const options = parseArguments(args);
  if (options.help) {
    process.stdout.write(help);
    return;
  }
  const extension = extname(options.source);
  const Parser = parsers.get(extension.toLowerCase());
  if (!Parser) throw new Error('Extensão sem suporte: ' + (extension || '(ausente)'));
  const Reporter = reporters.get(options.format.toLowerCase());
  if (!Reporter) throw new Error('Formato sem suporte: ' + options.format);
  const parser = new Parser();
  const reporter = new Reporter();
  const cidades = carregarCidades(options.source, parser);
  console.log(reporter.reportar(cidades));
}

try {
  main(process.argv.slice(2));
} catch (error) {
  console.error('Erro: ' + error.message);
  process.exitCode = 1;
}
