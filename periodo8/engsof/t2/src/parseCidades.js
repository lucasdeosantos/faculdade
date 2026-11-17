function normalizeNames(records, key) {
  if (!Array.isArray(records)) throw new Error('Dados devem formar uma lista.');
  return records.map((record, index) => {
    if (!record || typeof record[key] !== 'string' || !record[key].trim()) {
      throw new Error(`Registro ${index + 1}: campo ${key} deve ser texto não vazio.`);
    }
    return { name: record[key] };
  });
}

export function parseCidadesJSON(text) {
  return normalizeNames(JSON.parse(text), 'Nome');
}

function parseCSVRows(text) {
  const rows = [];
  let row = [], field = '', state = 'start';
  const finishField = () => { row.push(field); field = ''; state = 'start'; };
  const finishRow = () => { finishField(); rows.push(row); row = []; };
  for (let i = 0; i < text.length; i++) {
    const c = text[i];
    if (state === 'quoted') {
      if (c === '"') {
        if (text[i + 1] === '"') { field += '"'; i++; }
        else state = 'closed';
      } else field += c;
      continue;
    }
    if (c === ',') { finishField(); continue; }
    if (c === '\n' || c === '\r') {
      if (c === '\r' && text[i + 1] === '\n') i++;
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

export function parseCidadesCSV(text) {
  const [header, ...rows] = parseCSVRows(text);
  if (!header || header.filter(key => key === 'NOME').length !== 1) {
    throw new Error('CSV: cabeçalho deve conter uma coluna NOME.');
  }
  if (new Set(header).size !== header.length) throw new Error('CSV: cabeçalho duplicado.');
  const records = rows.map((row, index) => {
    if (row.length !== header.length) {
      throw new Error(`CSV: linha ${index + 2} com número de campos inválido.`);
    }
    return Object.fromEntries(header.map((key, i) => [key, row[i]]));
  });
  return normalizeNames(records, 'NOME');
}
