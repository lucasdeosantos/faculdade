function normalizeNames(cidades, key) {
  if (!Array.isArray(cidades)) throw new Error('Dados devem formar uma lista.');
  return cidades.map((cidade, index) => {
    if (!cidade || typeof cidade[key] !== 'string' || !cidade[key].trim()) {
      throw new Error(`Registro ${index + 1}: campo ${key} deve ser texto não vazio.`);
    }
    return { name: cidade[key] };
  });
}

export function parseCidadesJSON(texto) {
  const cidades = JSON.parse(texto);
  if (Array.isArray(cidades)) return normalizeNames(cidades, 'Nome');
  if (cidades && Array.isArray(cidades.estados)) {
    return cidades.estados.flatMap(estado =>
      estado.cidades.map(cidade => ({ name: cidade }))
    );
  }
  throw new Error('JSON: estrutura de cidades inválida.');
}

function parseCSVRows(texto) {
  const rows = [];
  let row = [], field = '', state = 'start';
  const finishField = () => { row.push(field); field = ''; state = 'start'; };
  const finishRow = () => { finishField(); rows.push(row); row = []; };
  for (let i = 0; i < texto.length; i++) {
    const c = texto[i];
    if (state === 'quoted') {
      if (c === '"') {
        if (texto[i + 1] === '"') { field += '"'; i++; }
        else state = 'closed';
      } else field += c;
      continue;
    }
    if (c === ',') { finishField(); continue; }
    if (c === '\n' || c === '\r') {
      if (c === '\r' && texto[i + 1] === '\n') i++;
      finishRow(); continue;
    }
    if (state === 'closed') throw new Error('CSV: caractere após fechamento de aspas.');
    if (c === '"') {
      if (state !== 'start') throw new Error('CSV: aspas em campo não delimitado.');
      state = 'quoted';
    } else { field += c; state = 'plain'; }
  }
  if (state === 'quoted') throw new Error('CSV: aspas não fechadas.');
  if (row.length || field.length || state !== 'start') finishRow();
  return rows;
}

export function parseCidadesCSV(texto) {
  const [header, ...rows] = parseCSVRows(texto);
  if (!header || header.filter(key => key === 'NOME').length !== 1) {
    throw new Error('CSV: cabeçalho deve conter uma coluna NOME.');
  }
  if (new Set(header).size !== header.length) throw new Error('CSV: cabeçalho duplicado.');
  const cidades = rows.map((row, index) => {
    if (row.length !== header.length) {
      throw new Error(`CSV: linha ${index + 2} com número de campos inválido.`);
    }
    return Object.fromEntries(header.map((key, i) => [key, row[i]]));
  });
  return normalizeNames(cidades, 'NOME');
}