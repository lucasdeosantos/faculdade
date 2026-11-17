import { extname } from 'node:path';
import { lerCidades } from './src/lerCidades.js';
import { parseCidadesJSON, parseCidadesCSV } from './src/parseCidades.js';
import { HtmlReporter } from './src/reporters/HtmlReporter.js';
import { TxtReporter } from './src/reporters/TxtReporter.js';
import { MarkdownReporter } from './src/reporters/MarkdownReporter.js';

const help = `Uso: node index.js --input <arquivo.json|arquivo.csv> --format <html|txt|markdown>
     node index.js --help
`;

function parseArguments(args) {
  if (args.length === 1 && args[0] === '--help') return { help: true };
  const options = {};
  for (let i = 0; i < args.length; i += 2) {
    const flag = args[i];
    if (!['--input', '--format'].includes(flag) || !args[i + 1] || args[i + 1].startsWith('-')) {
      throw new Error('Argumentos inválidos. Consulte --help.');
    }
    const key = flag.slice(2);
    if (key in options) throw new Error(`Argumento repetido: ${flag}`);
    options[key] = args[i + 1];
  }
  if (!options.input) throw new Error('Informe --input. Consulte --help.');
  if (!options.format) throw new Error('Informe --format. Consulte --help.');
  return options;
}

const parsers = new Map([['.json', parseCidadesJSON], ['.csv', parseCidadesCSV]]);
const reporters = new Map([['html', HtmlReporter], ['txt', TxtReporter], ['markdown', MarkdownReporter]]);
const options = parseArguments(process.argv.slice(2));
if (options.help) process.stdout.write(help);
else {
  const extension = extname(options.input);
  const parse = parsers.get(extension.toLowerCase());
  const Reporter = reporters.get(options.format);
  if (!parse) throw new Error(`Extensão sem suporte: ${extension || '(ausente)'}`);
  if (!Reporter) throw new Error(`Formato sem suporte: ${options.format}`);
  const reporter = new Reporter();
  const cidades = lerCidades(options.input, parse);
  const relatorio = reporter.reportar(cidades);
  console.log(relatorio);
}
