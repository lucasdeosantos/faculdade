import hashlib
import hmac
import secrets
import sqlite3
from models import (texto, papel, Hackathon, Equipe, Organizador, Participante, Mentor, Jurado)


def senha_hash(senha, salt=None):
    salt = salt or secrets.token_hex(16)
    digest = hashlib.pbkdf2_hmac('sha256', senha.encode(), bytes.fromhex(salt), 260000).hex()
    return salt + ':' + digest


class Sistema:
    def __init__(self, db):
        self.db = db

    def cadastrar(self, nome, email, senha, *, organizador=False):
        nome, email = texto(nome), texto(email).lower()
        if '@' not in email or not all(email.split('@')) or email.count('@') != 1:
            raise ValueError('E-mail inválido.')
        if len(senha) < 6:
            raise ValueError('A senha precisa ter pelo menos 6 caracteres.')
        try:
            with self.db:
                return self.db.execute('INSERT INTO usuarios(nome,email,senha,organizador) VALUES(?,?,?,?)',
                                       (nome, email, senha_hash(senha), int(organizador))).lastrowid
        except sqlite3.IntegrityError:
            raise ValueError('E-mail já cadastrado.') from None

    def login(self, email, senha):
        u = self.db.execute('SELECT * FROM usuarios WHERE email=?', (email.strip().lower(),)).fetchone()
        if not u or not hmac.compare_digest(u['senha'], senha_hash(senha, u['senha'].split(':')[0])):
            raise ValueError('E-mail ou senha incorretos.')
        return u['id']

    def obter(self, tabela, ident):
        return self.db.obter(tabela, ident)

    def papel(self, u, h, esperado=None):
        return papel(self.db, u, h, esperado)

    def hackathon(self, h):
        return Hackathon(**dict(self.obter('hackathons', h)), db=self.db)

    def equipe(self, e):
        return Equipe(**dict(self.obter('equipes', e)), db=self.db)

    def hackathons(self):
        return [Hackathon(**dict(r), db=self.db) for r in self.db.execute('SELECT * FROM hackathons ORDER BY id')]

    def organizador(self, u):
        usuario = self.obter('usuarios', u)
        if not usuario['organizador']:
            raise ValueError('Somente organizadores podem gerenciar hackathons.')
        return Organizador(u, usuario['nome'], usuario['email'], db=self.db)

    def atuacao(self, u, h, esperado=None):
        atual = self.papel(u, h, esperado)
        usuario = self.obter('usuarios', u)
        classe = {'participante': Participante, 'mentor': Mentor, 'jurado': Jurado}[atual]
        return classe(u, usuario['nome'], usuario['email'], h, db=self.db)

    def participante(self, u, h):
        return self.atuacao(u, h, 'participante')

    def mentor(self, u, h):
        return self.atuacao(u, h, 'mentor')

    def jurado(self, u, h):
        return self.atuacao(u, h, 'jurado')
