export function normalizarNomes(cidades, key) {
  if (!Array.isArray(cidades)) throw new Error('Dados devem formar uma lista.');
  
  return cidades.map((cidade, index) => {
    if (!cidade || typeof cidade[key] !== 'string' || !cidade[key].trim()) {
      throw new Error(`Registro ${index + 1}: campo ${key} deve ser texto não vazio.`);
    }
    return { name: cidade[key] };
  });
}