import sqlite3
from dataclasses import dataclass, field
from database import Database
from datetime import date


def papel(db, u, h, esperado=None):
    r = db.inscricao(u, h)
    if not r or (esperado and r['papel'] != esperado):
        raise ValueError('Você não possui o papel necessário neste hackathon.')
    return r['papel']


def texto(valor):
    valor = valor.strip()
    if not valor:
        raise ValueError('Preencha todos os campos.')
    return valor


@dataclass
class Organizador:
    db: Database = field(repr=False, compare=False, kw_only=True)
    id: int
    nome: str
    email: str

    def validar_propriedade(self, h):
        u = self.id
        if not self.db.obter('usuarios', u)['organizador']:
            raise ValueError('Somente organizadores podem gerenciar hackathons.')
        if self.db.obter('hackathons', h)['organizador_id'] != u:
            raise ValueError('Somente o organizador deste hackathon pode alterá-lo.')

    def salvar_hackathon(self, nome, inicio, fim, limite, h=None):
        u = self.id
        if not self.db.obter('usuarios', u)['organizador']:
            raise ValueError('Somente organizadores podem gerenciar hackathons.')
        nome = texto(nome)
        try:
            inicio, fim = date.fromisoformat(inicio), date.fromisoformat(fim)
        except ValueError:
            raise ValueError('Use datas válidas no formato AAAA-MM-DD.') from None
        if fim < inicio or type(limite) is not int or limite <= 0:
            raise ValueError('Verifique o período e o limite positivo de equipes.')
        if h is not None:
            self.validar_propriedade(h)
            if limite < len(Hackathon(db=self.db, **dict(self.db.obter('hackathons', h))).equipes()):
                raise ValueError('O limite não pode ser menor que o total de equipes existentes.')
        with self.db:
            if h is None:
                return self.db.execute('INSERT INTO hackathons(organizador_id,nome,data_inicio,data_fim,max_equipes) VALUES(?,?,?,?,?)',
                                       (u, nome, inicio.isoformat(), fim.isoformat(), limite)).lastrowid
            self.db.execute('UPDATE hackathons SET nome=?,data_inicio=?,data_fim=?,max_equipes=? WHERE id=?',
                            (nome, inicio.isoformat(), fim.isoformat(), limite, h))
            return h

    def remover_hackathon(self, h):
        u = self.id
        self.validar_propriedade(h)
        with self.db:
            self.db.execute('DELETE FROM hackathons WHERE id=?', (h,))

    def visualizar_hackathons(self):
        if not self.db.obter('usuarios', self.id)['organizador']:
            raise ValueError('Somente organizadores podem gerenciar hackathons.')
        return [Hackathon(**dict(r), db=self.db) for r in self.db.execute(
            'SELECT * FROM hackathons WHERE organizador_id=? ORDER BY id', (self.id,))]

    def criar_hackathon(self, nome, inicio, fim, limite):
        return self.salvar_hackathon(nome, inicio, fim, limite)

    def editar_hackathon(self, h, nome, inicio, fim, limite):
        return self.salvar_hackathon(nome, inicio, fim, limite, h)


@dataclass
class Hackathon:
    db: Database = field(repr=False, compare=False, kw_only=True)
    id: int
    organizador_id: int
    nome: str
    data_inicio: str
    data_fim: str
    max_equipes: int

    def equipes(self, busca=""):
        h = self.id
        return [Equipe(**dict(r), db=self.db) for r in self.db.execute(
            'SELECT * FROM equipes WHERE hackathon_id=? AND instr(lower(nome),lower(?))>0 ORDER BY nome', (h, busca))]

    def ranking(self):
        # média decrescente, empates compartilham posição, não avaliadas ao final.
        rows = self.db.execute('''
            SELECT e.id AS equipe_id, e.nome AS equipe, p.titulo AS projeto,
                   AVG(a.nota) AS media, COUNT(a.id) AS avaliacoes
            FROM equipes e
            LEFT JOIN projetos p ON p.equipe_id=e.id
            LEFT JOIN avaliacoes a ON a.projeto_id=p.id
            WHERE e.hackathon_id=?
            GROUP BY e.id, e.nome, p.id, p.titulo
            ORDER BY media IS NULL, media DESC, e.nome COLLATE NOCASE, e.id
        ''', (self.id,)).fetchall()
        resultado, anterior, posicao = [], None, None
        for indice, row in enumerate(rows, 1):
            item = dict(row)
            if item['media'] is not None:
                if item['media'] != anterior:
                    posicao = indice
                anterior = item['media']
                item['posicao'] = posicao
            else:
                item['posicao'] = None
            resultado.append(item)
        return resultado

    def entrar(self, u, papel):
        h = self.id
        self.db.obter('hackathons', h)
        if self.db.obter('usuarios', u)['organizador']:
            raise ValueError('O organizador utiliza o menu de organização.')
        if papel not in ('participante', 'mentor', 'jurado'):
            raise ValueError('Papel inválido.')
        if self.db.execute('SELECT 1 FROM inscricoes WHERE usuario_id=? AND hackathon_id=?', (u, h)).fetchone():
            raise ValueError('Saia do hackathon antes de escolher outro papel.')
        with self.db:
            self.db.execute('INSERT INTO inscricoes VALUES(?,?,?)', (u, h, papel))


@dataclass
class Usuario:
    db: Database = field(repr=False, compare=False, kw_only=True)
    id: int
    nome: str
    email: str
    hackathon_id: int

    def validar(self):
        papel(self.db, self.id, self.hackathon_id, self.PAPEL)

    def sair_hackathon(self):
        self.validar()
        lider = self.db.execute('SELECT 1 FROM equipes WHERE lider_id=? AND hackathon_id=?',
                                (self.id, self.hackathon_id)).fetchone()
        if lider:
            raise ValueError('Transfira a liderança antes de sair; se estiver sozinho, exclua a equipe.')
        with self.db:
            self.db.execute('DELETE FROM membros WHERE usuario_id=? AND hackathon_id=?', (self.id, self.hackathon_id))
            self.db.execute('DELETE FROM inscricoes WHERE usuario_id=? AND hackathon_id=?', (self.id, self.hackathon_id))


@dataclass
class Participante(Usuario):
    PAPEL = 'participante'

    def visualizar_equipe(self):
        self.validar()
        u, h = self.id, self.hackathon_id
        r = self.db.execute('SELECT e.* FROM equipes e JOIN membros m ON m.equipe_id=e.id WHERE m.usuario_id=? AND m.hackathon_id=?', (u, h)).fetchone()
        return Equipe(**dict(r), db=self.db) if r else None

    def criar_equipe(self, nome):
        u, h = self.id, self.hackathon_id
        nome = texto(nome)
        papel(self.db, u, h, 'participante')
        with self.db:
            self.db.execute('BEGIN IMMEDIATE')
            if self.visualizar_equipe():
                raise ValueError('Você já está em uma equipe neste hackathon.')
            if len(Hackathon(db=self.db, **dict(self.db.obter('hackathons', h))).equipes()) >= self.db.obter('hackathons', h)['max_equipes']:
                raise ValueError('Limite de equipes atingido.')
            try:
                e = self.db.execute('INSERT INTO equipes(hackathon_id,lider_id,nome) VALUES(?,?,?)', (h, u, nome)).lastrowid
            except sqlite3.IntegrityError:
                raise ValueError('Já existe uma equipe com esse nome.') from None
            self.db.execute('INSERT INTO membros VALUES(?,?,?)', (u, h, e))
            return e

    def entrar_equipe(self, e):
        u, h = self.id, self.hackathon_id
        papel(self.db, u, h, 'participante')
        if self.db.obter('equipes', e)['hackathon_id'] != h:
            raise ValueError('Equipe de outro hackathon.')
        try:
            with self.db:
                self.db.execute('INSERT INTO membros VALUES(?,?,?)', (u, h, e))
        except sqlite3.IntegrityError:
            raise ValueError('Você já está em uma equipe neste hackathon.') from None

    def procurar_equipes(self, nome=''):
        self.validar()
        return Hackathon(db=self.db, **dict(self.db.obter('hackathons', self.hackathon_id))).equipes(nome)


@dataclass
class Equipe:
    db: Database = field(repr=False, compare=False, kw_only=True)
    id: int
    hackathon_id: int
    lider_id: int
    nome: str

    def validar_lider(self, u):
        e = self.id
        equipe = self.db.obter('equipes', e)
        papel(self.db, u, equipe['hackathon_id'], 'participante')
        if equipe['lider_id'] != u:
            raise ValueError('Somente o líder pode executar esta ação.')

    def visualizar_participantes(self):
        e = self.id
        return [dict(r) for r in self.db.execute('SELECT u.id,u.nome,u.email FROM usuarios u JOIN membros m ON m.usuario_id=u.id WHERE m.equipe_id=? ORDER BY u.nome', (e,))]

    def renomear(self, u, nome):
        e = self.id
        self.validar_lider(u)
        try:
            with self.db:
                self.db.execute('UPDATE equipes SET nome=? WHERE id=?', (texto(nome), e))
            self.nome = nome.strip()
        except sqlite3.IntegrityError:
            raise ValueError('Já existe uma equipe com esse nome.') from None

    def _alterar_membro(self, u, alvo, transferir=False):
        e = self.id
        self.validar_lider(u)
        if alvo == u or not any(p['id'] == alvo for p in self.visualizar_participantes()):
            raise ValueError('Escolha outro participante da equipe.')
        with self.db:
            if transferir:
                self.db.execute('UPDATE equipes SET lider_id=? WHERE id=?', (alvo, e))
                self.lider_id = alvo
            else:
                self.db.execute('DELETE FROM membros WHERE usuario_id=? AND equipe_id=?', (alvo, e))

    def remover_participante(self, u, alvo):
        self._alterar_membro(u, alvo)

    def transferir_lideranca(self, u, alvo):
        self._alterar_membro(u, alvo, transferir=True)

    def excluir(self, u):
        e = self.id
        self.validar_lider(u)
        with self.db:
            self.db.execute('DELETE FROM equipes WHERE id=?', (e,))

    def visualizar_projeto(self):
        e = self.id
        r = self.db.execute('SELECT * FROM projetos WHERE equipe_id=?', (e,)).fetchone()
        return Projeto(**dict(r)) if r else None

    def salvar_projeto(self, u, titulo, descricao, area):
        e = self.id
        self.validar_lider(u)
        with self.db:
            self.db.execute('''INSERT INTO projetos(equipe_id,titulo,descricao,area_tematica) VALUES(?,?,?,?)
                ON CONFLICT(equipe_id) DO UPDATE SET titulo=excluded.titulo,descricao=excluded.descricao,area_tematica=excluded.area_tematica''',
                (e, texto(titulo), texto(descricao), texto(area)))

    def excluir_projeto(self, u):
        e = self.id
        self.validar_lider(u)
        with self.db:
            self.db.execute('DELETE FROM projetos WHERE equipe_id=?', (e,))


@dataclass
class Projeto:
    id: int
    equipe_id: int
    titulo: str
    descricao: str
    area_tematica: str


@dataclass
class Mentor(Usuario):
    PAPEL = 'mentor'

    def visualizar_mentorias(self):
        u, h = self.id, self.hackathon_id
        papel(self.db, u, h, 'mentor')
        return [Mentoria(**dict(r)) for r in self.db.execute('SELECT m.* FROM mentorias m JOIN equipes e ON e.id=m.equipe_id WHERE m.mentor_id=? AND e.hackathon_id=?', (u, h))]

    def salvar_mentoria(self, e, comentarios):
        u, h = self.id, self.hackathon_id
        papel(self.db, u, h, 'mentor')
        if self.db.obter('equipes', e)['hackathon_id'] != h:
            raise ValueError('Equipe de outro hackathon.')
        with self.db:
            self.db.execute('''INSERT INTO mentorias(mentor_id,equipe_id,comentarios) VALUES(?,?,?)
                ON CONFLICT(mentor_id,equipe_id) DO UPDATE SET comentarios=excluded.comentarios''', (u, e, texto(comentarios)))

    def iniciar_mentoria(self, e, comentarios):
        if any(m.equipe_id == e for m in self.visualizar_mentorias()):
            raise ValueError('Mentoria existente. Use Editar comentários.')
        self.salvar_mentoria(e, comentarios)

    def editar_comentarios(self, mentoria_id, comentarios):
        mentoria = next((m for m in self.visualizar_mentorias() if m.id == mentoria_id), None)
        if mentoria is None:
            raise ValueError('Mentoria não encontrada entre suas mentorias.')
        self.salvar_mentoria(mentoria.equipe_id, comentarios)


@dataclass
class Mentoria:
    id: int
    mentor_id: int
    equipe_id: int
    comentarios: str


@dataclass
class Jurado(Usuario):
    PAPEL = 'jurado'

    def visualizar_projetos(self, filtro="todos"):
        u, h = self.id, self.hackathon_id
        papel(self.db, u, h, 'jurado')
        rows = self.db.execute('''SELECT p.*,e.nome AS equipe,a.nota,a.comentario FROM projetos p
            JOIN equipes e ON e.id=p.equipe_id LEFT JOIN avaliacoes a ON a.projeto_id=p.id AND a.jurado_id=?
            WHERE e.hackathon_id=? ORDER BY p.id''', (u, h))
        return [dict(r) for r in rows if filtro == 'todos' or (r['nota'] is not None) == (filtro == 'avaliados')]

    def salvar_avaliacao(self, p, nota, comentario):
        u, h = self.id, self.hackathon_id
        papel(self.db, u, h, 'jurado')
        projeto = self.db.obter('projetos', p)
        if self.db.obter('equipes', projeto['equipe_id'])['hackathon_id'] != h:
            raise ValueError('Projeto de outro hackathon.')
        if type(nota) is not int or not -(2**63) <= nota < 2**63:
            raise ValueError('A nota deve ser um inteiro válido para SQLite.')
        with self.db:
            self.db.execute('''INSERT INTO avaliacoes(jurado_id,projeto_id,nota,comentario) VALUES(?,?,?,?)
                ON CONFLICT(jurado_id,projeto_id) DO UPDATE SET nota=excluded.nota,comentario=excluded.comentario''',
                (u, p, nota, texto(comentario)))

    def avaliar_projeto(self, p, nota, comentario):
        if not any(r['id'] == p for r in self.visualizar_projetos('pendentes')):
            raise ValueError('Escolha um projeto ainda não avaliado deste hackathon.')
        self.salvar_avaliacao(p, nota, comentario)

    def editar_avaliacao(self, p, nota, comentario):
        if not any(r['id'] == p for r in self.visualizar_projetos('avaliados')):
            raise ValueError('Você ainda não avaliou esse projeto neste hackathon.')
        self.salvar_avaliacao(p, nota, comentario)


@dataclass
class Avaliacao:
    id: int
    jurado_id: int
    projeto_id: int
    nota: int
    comentario: str

