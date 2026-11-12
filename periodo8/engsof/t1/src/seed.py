import secrets
from services import Sistema, senha_hash

def preparar(db):
    senha = 'Adm123!'
    if not db.execute('SELECT 1 FROM usuarios WHERE organizador=1').fetchone():
        Sistema(db).cadastrar('Organizador', 'organizador@local', senha, organizador=True)
    else:
        with db:
            for organizador in db.execute('SELECT id FROM usuarios WHERE organizador=1').fetchall():
                db.execute('UPDATE usuarios SET senha=? WHERE id=?', (senha_hash(senha), organizador['id']))
    print('\nOrganizador cadastrado:')
    for organizador in db.execute('SELECT nome,email FROM usuarios WHERE organizador=1 ORDER BY id'):
        print(f"{organizador['nome']} | E-mail: {organizador['email']}")
        print('Senha: ' + senha)
    popular_demonstracao(db)


def popular_demonstracao(db):
    with db:
        db.execute('CREATE TABLE IF NOT EXISTS seeds (nome TEXT PRIMARY KEY)')
        db.execute('BEGIN IMMEDIATE')
        if db.execute("SELECT 1 FROM seeds WHERE nome='demonstracao_v1'").fetchone():
            print('Demonstração já aplicada. Os dados de exemplo não foram duplicados ou redefinidos.')
            mostrar_demonstracao(db)
            return
        organizador = db.execute('SELECT id FROM usuarios WHERE organizador=1 ORDER BY id LIMIT 1').fetchone()[0]
        h = db.execute('''INSERT INTO hackathons(organizador_id,nome,data_inicio,data_fim,max_equipes)
            VALUES(?,?,?,?,?)''', (organizador, 'Hackathon Inovação — Demonstração', '2026-09-01', '2026-09-30', 5)).lastrowid
        sufixo = ''
        apelidos = ('ana', 'bruno', 'carla', 'diego', 'marina', 'rafael', 'julia', 'pedro')
        while any(db.execute('SELECT 1 FROM usuarios WHERE email=?', (f'{a}{sufixo}@demo.local',)).fetchone() for a in apelidos):
            sufixo = '-' + secrets.token_hex(3)
        ids = []
        for apelido, nome, papel in zip(apelidos,
                ('Ana', 'Bruno', 'Carla', 'Diego', 'Marina', 'Rafael', 'Júlia', 'Pedro'),
                ('participante',) * 4 + ('mentor',) * 2 + ('jurado',) * 2):
            u = db.execute('INSERT INTO usuarios(nome,email,senha) VALUES(?,?,?)',
                           (nome + ' (demo)', f'{apelido}{sufixo}@demo.local', senha_hash('Demo123!'))).lastrowid
            db.execute('INSERT INTO inscricoes VALUES(?,?,?)', (u, h, papel))
            ids.append(u)
        projetos = []
        equipes = []
        for lider, membro, nome, titulo, descricao, area in (
            (ids[0], ids[1], 'EcoTech', 'Recicla Fácil', 'Conecta moradores a pontos de coleta seletiva.', 'Sustentabilidade'),
            (ids[2], ids[3], 'EduCode', 'Aprender Juntos', 'Organiza grupos de estudo colaborativos.', 'Educação'),
        ):
            e = db.execute('INSERT INTO equipes(hackathon_id,lider_id,nome) VALUES(?,?,?)', (h, lider, nome)).lastrowid
            equipes.append(e)
            db.executemany('INSERT INTO membros VALUES(?,?,?)', ((lider, h, e), (membro, h, e)))
            projetos.append(db.execute('INSERT INTO projetos(equipe_id,titulo,descricao,area_tematica) VALUES(?,?,?,?)',
                                       (e, titulo, descricao, area)).lastrowid)
        db.executemany('INSERT INTO mentorias(mentor_id,equipe_id,comentarios) VALUES(?,?,?)', (
            (ids[4], equipes[0], 'Validar os pontos de coleta com moradores.'),
            (ids[5], equipes[1], 'Testar a proposta com um grupo pequeno de estudantes.'),
        ))
        db.executemany('INSERT INTO avaliacoes(jurado_id,projeto_id,nota,comentario) VALUES(?,?,?,?)', (
            (ids[6], projetos[0], 9, 'Proposta clara e impacto relevante.'),
            (ids[6], projetos[1], 8, 'Boa proposta; detalhar a validação.'),
            (ids[7], projetos[0], 8, 'Solução viável para o problema apresentado.'),
        ))
        db.execute("INSERT INTO seeds VALUES('demonstracao_v1')")
    print('\nDemonstração criada: 1 hackathon, 2 equipes, 4 participantes, 2 mentores e 2 jurados.')
    print('Contas de demonstração criadas:')
    for apelido, papel in zip(apelidos, ('Participante',) * 4 + ('Mentor',) * 2 + ('Jurado',) * 2):
        print(f'{apelido}{sufixo}@demo.local | {papel}')
    print('Senha das contas de demonstração: Demo123!\n')


def mostrar_demonstracao(db):
    eventos = db.execute("SELECT id,nome FROM hackathons WHERE nome=?",
                        ('Hackathon Inovação — Demonstração',)).fetchall()
    if not eventos:
        print('O hackathon de demonstração foi excluído ou renomeado; não será recriado.')
    for evento in eventos:
        h = evento['id']
        equipes = db.execute('SELECT count(*) FROM equipes WHERE hackathon_id=?', (h,)).fetchone()[0]
        totais = dict(db.execute('SELECT papel,count(*) FROM inscricoes WHERE hackathon_id=? GROUP BY papel', (h,)))
        print(f"\nDemonstração disponível: {evento['nome']} (ID {h})")
        print(f"{equipes} equipes, {totais.get('participante', 0)} participantes, "
              f"{totais.get('mentor', 0)} mentores e {totais.get('jurado', 0)} jurados.")
    print('Contas de demonstração cadastradas:')
    contas = db.execute('''SELECT u.email, GROUP_CONCAT(DISTINCT i.papel) AS papeis
        FROM usuarios u LEFT JOIN inscricoes i ON i.usuario_id=u.id
        WHERE u.nome LIKE '% (demo)' AND u.email LIKE '%@demo.local'
        GROUP BY u.id ORDER BY u.id''').fetchall()
    for conta in contas:
        print(f"{conta['email']} | {conta['papeis'] or 'Sem inscrição atual'}")
    if not contas:
        print('Nenhuma conta de demonstração encontrada.')
    print('Senha inicial das contas de demonstração: Demo123!\n')


if __name__ == '__main__':
    from database import Database
    db = Database()
    try:
        preparar(db)
    finally:
        db.close()
