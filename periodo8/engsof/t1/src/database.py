import sqlite3
from pathlib import Path


class Database:
    def __init__(self, caminho=None):
        self._conexao = sqlite3.connect(caminho or Path(__file__).with_name('hackathon.db'))
        self._conexao.row_factory = sqlite3.Row
        try:
            self.execute('PRAGMA foreign_keys = ON')
            self.criar_tabelas()
        except Exception:
            self.close()
            raise

    def criar_tabelas(self):
        self._conexao.executescript('''
    CREATE TABLE IF NOT EXISTS usuarios (
        id INTEGER PRIMARY KEY, nome TEXT NOT NULL, email TEXT NOT NULL UNIQUE,
        senha TEXT NOT NULL, organizador INTEGER NOT NULL DEFAULT 0);
    CREATE TABLE IF NOT EXISTS hackathons (
        id INTEGER PRIMARY KEY, organizador_id INTEGER NOT NULL REFERENCES usuarios(id),
        nome TEXT NOT NULL, data_inicio TEXT NOT NULL, data_fim TEXT NOT NULL,
        max_equipes INTEGER NOT NULL CHECK(max_equipes > 0));
    CREATE TABLE IF NOT EXISTS inscricoes (
        usuario_id INTEGER REFERENCES usuarios(id),
        hackathon_id INTEGER REFERENCES hackathons(id) ON DELETE CASCADE,
        papel TEXT NOT NULL CHECK(papel IN ('participante','mentor','jurado')),
        PRIMARY KEY(usuario_id, hackathon_id));
    CREATE TABLE IF NOT EXISTS equipes (
        id INTEGER PRIMARY KEY, hackathon_id INTEGER NOT NULL REFERENCES hackathons(id) ON DELETE CASCADE,
        lider_id INTEGER NOT NULL REFERENCES usuarios(id), nome TEXT NOT NULL COLLATE NOCASE,
        UNIQUE(hackathon_id, nome));
    CREATE TABLE IF NOT EXISTS membros (
        usuario_id INTEGER REFERENCES usuarios(id),
        hackathon_id INTEGER REFERENCES hackathons(id) ON DELETE CASCADE,
        equipe_id INTEGER NOT NULL REFERENCES equipes(id) ON DELETE CASCADE,
        PRIMARY KEY(usuario_id, hackathon_id));
    CREATE TABLE IF NOT EXISTS projetos (
        id INTEGER PRIMARY KEY, equipe_id INTEGER NOT NULL UNIQUE REFERENCES equipes(id) ON DELETE CASCADE,
        titulo TEXT NOT NULL, descricao TEXT NOT NULL, area_tematica TEXT NOT NULL);
    CREATE TABLE IF NOT EXISTS mentorias (
        id INTEGER PRIMARY KEY, mentor_id INTEGER NOT NULL REFERENCES usuarios(id),
        equipe_id INTEGER NOT NULL REFERENCES equipes(id) ON DELETE CASCADE,
        comentarios TEXT NOT NULL, UNIQUE(mentor_id, equipe_id));
    CREATE TABLE IF NOT EXISTS avaliacoes (
        id INTEGER PRIMARY KEY, jurado_id INTEGER NOT NULL REFERENCES usuarios(id),
        projeto_id INTEGER NOT NULL REFERENCES projetos(id) ON DELETE CASCADE,
        nota INTEGER NOT NULL CHECK(typeof(nota) = 'integer'), comentario TEXT NOT NULL,
        UNIQUE(jurado_id, projeto_id));
    ''')

    def execute(self, sql, parametros=()):
        return self._conexao.execute(sql, parametros)

    def executemany(self, sql, registros):
        return self._conexao.executemany(sql, registros)

    def inscricao(self, usuario_id, hackathon_id):
        return self.execute('SELECT papel FROM inscricoes WHERE usuario_id=? AND hackathon_id=?',
                            (usuario_id, hackathon_id)).fetchone()

    def __enter__(self):
        self._conexao.__enter__()
        return self

    def __exit__(self, tipo, erro, traceback):
        return self._conexao.__exit__(tipo, erro, traceback)

    def close(self):
        self._conexao.close()

    def obter(self, tabela, ident):
        if tabela not in ('usuarios', 'hackathons', 'equipes', 'projetos', 'mentorias', 'avaliacoes'):
            raise ValueError('Tabela inválida.')
        row = self.execute(f'SELECT * FROM {tabela} WHERE id=?', (ident,)).fetchone()
        if row is None:
            raise ValueError('Registro não encontrado.')
        return row
