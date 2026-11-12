from dataclasses import fields, is_dataclass
from getpass import getpass
import sqlite3
from database import Database
from services import Sistema


def inteiro(mensagem):
    try:
        return int(input(mensagem))
    except ValueError:
        raise ValueError('Digite um número inteiro.') from None


def mostrar(registros):
    registros = list(registros)
    if not registros:
        print('Nenhum registro encontrado.')
    for r in registros:
        dados = {f.name: getattr(r, f.name) for f in fields(r) if f.name != 'db'} if is_dataclass(r) else dict(r)
        print(' | '.join(f'{k}: {v if v is not None else "—"}' for k, v in dados.items()))


def menu(titulo, opcoes):
    print('\n=== ' + titulo + ' ===')
    for chave, descricao in opcoes.items():
        print(f'{chave} - {descricao}')
    escolha = input('Opção: ').strip()
    if escolha not in opcoes:
        raise ValueError('Opção inválida.')
    return escolha


def selecionar_id(registros, rotulo):
    registros = list(registros)
    mostrar(registros)
    if not registros:
        return None
    ids = {r.id if is_dataclass(r) else r['id'] for r in registros}
    while True:
        try:
            ident = inteiro(f'{rotulo} (0 volta ao menu): ')
        except ValueError as exc:
            print(exc)
            continue
        if ident == 0:
            return None
        if ident in ids:
            return ident
        print('ID não encontrado. Escolha um ID da lista ou 0 para voltar.')


def confirmar(descricao):
    return input(descricao + ' Digite SIM para confirmar: ').strip() == 'SIM'


class CLI:
    def __init__(self, sistema):
        self.s = sistema

    def executar(self):
        while True:
            try:
                op = menu('SISTEMA DE HACKATHONS', {'1': 'Cadastrar', '2': 'Login', '3': 'Sair'})
                if op == '3':
                    return
                if op == '1':
                    self.s.cadastrar(input('Nome: '), input('E-mail: '), getpass('Senha (mínimo 6 caracteres): '))
                    print('Cadastro realizado. Faça login.')
                else:
                    u = self.s.login(input('E-mail: '), getpass('Senha: '))
                    self.sessao(u)
            except (ValueError, sqlite3.Error) as exc:
                print('Erro:', exc)

    def sessao(self, u):
        organizador = self.s.obter('usuarios', u)['organizador']
        while True:
            try:
                if organizador:
                    if not self.organizador(u):
                        return
                else:
                    op = menu('HACKATHONS', {'1': 'Ver hackathons disponíveis', '2': 'Entrar em um hackathon',
                              '3': 'Meus hackathons / abrir', '0': 'Logout'})
                    if op == '0':
                        return
                    if op == '1':
                        mostrar(self.s.hackathons())
                    elif op == '2':
                        eventos = self.s.hackathons()
                        h = selecionar_id(eventos, 'ID do hackathon')
                        if h is None:
                            continue
                        if not any(evento.id == h for evento in eventos):
                            raise ValueError('Hackathon não encontrado. Escolha um ID da lista.')
                        if self.s.db.execute('SELECT 1 FROM inscricoes WHERE usuario_id=? AND hackathon_id=?', (u, h)).fetchone():
                            raise ValueError('Você já está inscrito. Use Meus hackathons / abrir.')
                        papel = menu('ESCOLHA SEU PAPEL', {'1': 'Participante', '2': 'Mentor', '3': 'Jurado'})
                        self.s.hackathon(h).entrar(u, {'1': 'participante', '2': 'mentor', '3': 'jurado'}[papel])
                        self.hackathon(u, h)
                    else:
                        rows = list(self.s.db.execute('SELECT h.id,h.nome,i.papel FROM hackathons h JOIN inscricoes i ON i.hackathon_id=h.id WHERE i.usuario_id=?', (u,)))
                        h = selecionar_id(rows, 'ID do hackathon')
                        if h is not None:
                            self.hackathon(u, h)
            except (ValueError, sqlite3.Error) as exc:
                print('Erro:', exc)

    def organizador(self, u):
        op = menu('ORGANIZADOR', {'1': 'Criar hackathon', '2': 'Visualizar meus hackathons',
                  '3': 'Editar hackathon', '4': 'Remover hackathon', '5': 'Ver ranking das equipes', '0': 'Logout'})
        if op == '0':
            return False
        if op == '2':
            mostrar(self.s.organizador(u).visualizar_hackathons())
        elif op == '5':
            h = selecionar_id(self.s.hackathons(), 'ID do hackathon')
            if h is not None:
                self.ranking(h)
        elif op in ('1', '3'):
            h = None
            if op == '3':
                h = selecionar_id(self.s.organizador(u).visualizar_hackathons(), 'ID do hackathon')
                if h is None:
                    return True
                self.s.organizador(u).validar_propriedade(h)
            organizador = self.s.organizador(u)
            nome = input('Nome: ')
            inicio, fim = input('Início (AAAA-MM-DD): '), input('Término (AAAA-MM-DD): ')
            limite = inteiro('Máximo de equipes: ')
            if h is None:
                organizador.criar_hackathon(nome, inicio, fim, limite)
            else:
                organizador.editar_hackathon(h, nome, inicio, fim, limite)
            print('Hackathon salvo.')
        else:
            h = selecionar_id(self.s.organizador(u).visualizar_hackathons(), 'ID do hackathon')
            if h is None:
                return True
            self.s.organizador(u).validar_propriedade(h)
            if confirmar('Excluir o hackathon e todos os seus dados, incluindo mentorias e avaliações?'):
                self.s.organizador(u).remover_hackathon(h)
                print('Hackathon excluído.')
        return True

    def hackathon(self, u, h):
        self.s.papel(u, h)
        while True:
            try:
                papel = self.s.papel(u, h)
                opcoes = {
                    'participante': {'1': 'Criar equipe', '2': 'Procurar equipe pelo nome e entrar', '3': 'Visualizar minha equipe / gerenciar'},
                    'mentor': {'1': 'Visualizar minhas mentorias e comentários', '2': 'Iniciar mentoria', '3': 'Editar comentários'},
                    'jurado': {'1': 'Todos os projetos e minhas avaliações', '2': 'Projetos que já avaliei', '3': 'Projetos que ainda não avaliei', '4': 'Criar avaliação', '5': 'Editar minha avaliação'},
                }[papel]
                opcao_ranking = str(len(opcoes) + 1)
                opcao_saida = str(len(opcoes) + 2)
                op = menu(self.s.obter('hackathons', h)['nome'] + ' / ' + papel.upper(),
                          {**opcoes, opcao_ranking: 'Ver ranking das equipes', opcao_saida: 'Sair do hackathon', '0': 'Voltar'})
                if op == '0':
                    return
                if op == opcao_ranking:
                    self.ranking(h)
                    continue
                if op == opcao_saida:
                    if confirmar('Remover sua inscrição neste hackathon?'):
                        self.s.atuacao(u, h).sair_hackathon()
                        print('Você saiu do hackathon. Histórico de mentorias e avaliações preservado.')
                        return
                    continue
                if papel == 'participante':
                    self.participante(u, h, op)
                elif papel == 'mentor':
                    self.mentor(u, h, op)
                else:
                    self.jurado(u, h, op)
            except (ValueError, sqlite3.Error) as exc:
                print('Erro:', exc)

    def ranking(self, h):
        evento = self.s.hackathon(h)
        print(f'\n=== RANKING / {evento.nome} ===')
        print('Média das avaliações; empates compartilham posição. Sem avaliação: sem classificação.')
        linhas = []
        for item in evento.ranking():
            linhas.append({
                'Posição': item['posicao'] if item['posicao'] is not None else 'Sem classificação',
                'Equipe': item['equipe'],
                'Projeto': item['projeto'] or 'Sem projeto',
                'Média': f"{item['media']:.2f}" if item['media'] is not None else 'Sem avaliações',
                'Avaliações': item['avaliacoes'],
            })
        mostrar(linhas)

    def participante(self, u, h, op):
        if op == '1':
            self.s.participante(u, h).criar_equipe(input('Nome da equipe: '))
            print('Equipe criada. Você é o líder.')
        elif op == '2':
            equipes = self.s.participante(u, h).procurar_equipes(input('Nome ou parte do nome: '))
            e = selecionar_id(equipes, 'ID da equipe')
            if e is not None:
                self.s.participante(u, h).entrar_equipe(e)
                print('Você entrou na equipe.')
        else:
            self.equipe(u, h)

    def equipe(self, u, h):
        while True:
            e = self.s.participante(u, h).visualizar_equipe()
            if not e:
                print('Você ainda não está em uma equipe.')
                return
            mostrar([e])
            print('\nParticipantes:')
            mostrar(e.visualizar_participantes())
            print('\nProjeto:')
            p = e.visualizar_projeto()
            mostrar([p] if p else [])
            if e.lider_id != u:
                return
            try:
                op = menu('LÍDER', {'1': 'Editar nome da equipe', '2': 'Remover participante',
                    '3': 'Transferir liderança', '4': 'Criar / editar projeto', '5': 'Excluir projeto',
                    '6': 'Excluir equipe', '0': 'Voltar'})
                if op == '0':
                    return
                if op == '1':
                    e.renomear(u, input('Novo nome: '))
                elif op in ('2', '3'):
                    alvo = selecionar_id([m for m in e.visualizar_participantes() if m['id'] != u], 'ID do participante')
                    if alvo is None:
                        continue
                    if confirmar('Transferir liderança?' if op == '3' else 'Remover participante da equipe?'):
                        if op == '3':
                            e.transferir_lideranca(u, alvo)
                        else:
                            e.remover_participante(u, alvo)
                elif op == '4':
                    e.salvar_projeto(u, input('Título: '), input('Descrição: '), input('Área temática: '))
                elif op == '5':
                    if confirmar('Excluir projeto e suas avaliações?'):
                        e.excluir_projeto(u)
                elif confirmar('Excluir equipe, projeto, mentorias e avaliações associados?'):
                    e.excluir(u)
                    return
            except (ValueError, sqlite3.Error) as exc:
                print('Erro:', exc)

    def mentor(self, u, h, op):
        if op == '1':
            mostrar(self.s.mentor(u, h).visualizar_mentorias())
            return
        if op == '2':
            e = selecionar_id(self.s.hackathon(h).equipes(), 'ID da equipe')
            if e is None:
                return
            if any(m.equipe_id == e for m in self.s.mentor(u, h).visualizar_mentorias()):
                raise ValueError('Mentoria existente. Use Editar comentários.')
        else:
            mentorias = self.s.mentor(u, h).visualizar_mentorias()
            ident = selecionar_id(mentorias, 'ID da mentoria')
            if ident is None:
                return
            m = next((m for m in mentorias if m.id == ident), None)
            if m is None:
                raise ValueError('Mentoria não encontrada entre suas mentorias.')
            e = m.equipe_id
        comentarios = input('Comentários: ')
        if op == '2':
            self.s.mentor(u, h).iniciar_mentoria(e, comentarios)
        else:
            self.s.mentor(u, h).editar_comentarios(ident, comentarios)
        print('Mentoria salva.')

    def jurado(self, u, h, op):
        if op in ('1', '2', '3'):
            mostrar(self.s.jurado(u, h).visualizar_projetos({'1': 'todos', '2': 'avaliados', '3': 'pendentes'}[op]))
            return
        projetos = self.s.jurado(u, h).visualizar_projetos('pendentes' if op == '4' else 'avaliados')
        p = selecionar_id(projetos, 'ID do projeto')
        if p is None:
            return
        if not any(r['id'] == p for r in projetos):
            raise ValueError('Escolha um projeto da lista acima.')
        nota, comentario = inteiro('Nota inteira: '), input('Comentário: ')
        if op == '4':
            self.s.jurado(u, h).avaliar_projeto(p, nota, comentario)
        else:
            self.s.jurado(u, h).editar_avaliacao(p, nota, comentario)
        print('Avaliação salva.')


def main():
    db = Database()
    try:
        CLI(Sistema(db)).executar()
    except (EOFError, KeyboardInterrupt):
        print('\nSistema encerrado.')
    finally:
        db.close()


if __name__ == '__main__':
    main()
