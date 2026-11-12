import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);
        int op;

        Ecomp ecomp = new Ecomp();

        Cliente c = new Cliente("microhard", "12.345.678/0001-95", "microhard@email.com", "(123) 456-7890", "8h-17h");

        Desenvolvedor d1 = new Desenvolvedor("Lucas",  "123.456.789-09", "lucas@mail.com", "dev senior");
        Desenvolvedor d2 = new Desenvolvedor("Rafael", "987.654.321-00", "rafael@mail.com", "dev junior"); 
        Desenvolvedor d3 = new Desenvolvedor("Thiago", "741.852.963-40", "thiago@mail.com", "dev junior"); 
        Desenvolvedor d4 = new Desenvolvedor("Vitor",  "159.357.258-06", "vitor@mail.com",  "dev junior");
        
        ecomp.adicionarDesenvolvedor(d1);
        ecomp.adicionarDesenvolvedor(d2);
        ecomp.adicionarDesenvolvedor(d3);
        ecomp.adicionarDesenvolvedor(d4);

        do {
            System.out.println("===========================================");
            System.out.println("                SISTEMA ECOMP");
            System.out.println("===========================================");
            System.out.println("1. Cadastrar Projeto");
            System.out.println("2. Cadastrar Etapa de Desenvolvimento");
            System.out.println("3. Cadastrar Desenvolvedor no Projeto");
            System.out.println("4. Cadastrar Atividade");
            System.out.println("5. Cadastrar Notas Fiscais e Recibos");
            System.out.println("6. Cadastrar Ecomper");
            System.out.println("7. Gerar Relatório");
            System.out.println("8. Sair");
            System.out.println("===========================================");
            System.out.print("Escolha uma opção: ");

            while (!input.hasNextInt()) {
                System.out.print("Entrada inválida! Digite um número: ");
                input.next();
            }

            op = input.nextInt();
            input.nextLine();

            switch (op) {
                case 1: 
                {
                    System.out.println("===========================================");
                    System.out.println("             CADASTRAR PROJETO");
                    System.out.println("===========================================");

                    System.out.print("Data de início: ");
                    String dataInicio = input.nextLine();

                    System.out.print("Prazo: ");
                    String prazo = input.nextLine();

                    System.out.print("Descrição: ");
                    String descricao = input.nextLine();

                    System.out.print("Contrato: ");
                    String contrato = input.nextLine();

                    ecomp.cadastrarProjeto(c, dataInicio, prazo, descricao, contrato);
                    System.out.println("\nProjeto cadastrado com sucesso!");
                    break;
                }
                case 2:
                {
                    System.out.println("===========================================");
                    System.out.println("    CADASTRAR ETAPA DE DESENVOLVIMENTO");
                    System.out.println("===========================================");

                    System.out.println("Projetos disponíveis:");
                    ecomp.imprimirProjetos();

                    System.out.print("Digite o Id do projeto desejado: ");
                    int idProjeto = Integer.parseInt(input.nextLine());

                    Projeto proj = ecomp.selecionarProjeto(idProjeto);
                    if (proj == null) {
                        System.out.println("Projeto não encontrado!.");
                        break;
                    }

                    System.out.print("Cronograma: ");
                    String cronograma = input.nextLine();

                    System.out.print("Status: ");
                    String statusProjeto = input.nextLine();

                    ecomp.cadastrarEtapas(idProjeto, cronograma, statusProjeto);
                    System.out.println("\nEtapa cadastrada com sucesso!");
                    break;
                }
                case 3:
                {
                    System.out.println("===========================================");
                    System.out.println("    ADICIONAR DESENVOLVEDOR AO PROJETO");
                    System.out.println("===========================================");

                    System.out.println("Projetos cadastrados:");
                    ecomp.imprimirProjetos();

                    System.out.print("Digite o Id do projeto desejado: ");
                    int idProjeto = Integer.parseInt(input.nextLine());

                    Projeto proj = ecomp.selecionarProjeto(idProjeto);
                    if (proj == null) {
                        System.out.println("Projeto não encontrado!.");
                        break;
                    }

                    String continuar;
                    do {
                        System.out.println("Desenvolvedores:");
                        ecomp.imprimirDesenvolvedores();

                        System.out.print("Digite o ID do desenvolvedor: ");
                        int idDev = Integer.parseInt(input.nextLine());

                        boolean sucesso = ecomp.cadastrarDesenvolvedor(proj, idDev);

                        if (sucesso) {
                            System.out.println("Desenvolvedor adicionado com sucesso!");
                            System.out.println("Projeto atualizado:");
                            System.out.println(proj);
                        } else {
                            System.out.println("Desenvolvedor não encontrado!");
                        }

                        System.out.print("\nDeseja adicionar outro desenvolvedor ao projeto? (s/n): ");
                        continuar = input.nextLine().toLowerCase();

                    } while (continuar.equalsIgnoreCase("s"));

                    break;
                }
                case 4:
                {
                    System.out.println("===========================================");
                    System.out.println("          CADASTRAR NOVA ATIVIDADE");
                    System.out.println("===========================================");

                    System.out.print("Nome da atividade: ");
                    String nome = input.nextLine();

                    System.out.print("Duração da atividade: ");
                    String duracao = input.nextLine();

                    System.out.print("Local: ");
                    String local = input.nextLine();

                    System.out.print("Objetivo: ");
                    String objetivo = input.nextLine();

                    System.out.print("Resumo da Atividade: ");
                    String resumo = input.nextLine();

                    Atividade atividade = ecomp.cadastrarAtividade(nome, duracao, local, objetivo, resumo);

                    String continuar;
                    do {
                        System.out.println("Ecompers:");
                        ecomp.imprimirMembros();

                        System.out.print("Digite o ID do Ecomper participante: ");
                        int idEcomper = Integer.parseInt(input.nextLine());
    

                        if (ecomp.adicionarParticipanteAtividade(atividade, idEcomper)) {
                            System.out.println("Participante adicionado com sucesso!");
                        } else {
                            System.out.println("Ecomper não encontrado!");
                        }

                        System.out.print("\nDeseja adicionar outro participante? (s/n): ");
                        continuar = input.nextLine().toLowerCase();

                    } while (continuar.equalsIgnoreCase("s"));

                    ecomp.adicionarAtividade(atividade);
                    System.out.println("Atividade cadastrada e atualizada!");
                    break;
                }
                case 5:
                    {
                        System.out.println("===========================================");
                        System.out.println("     CADASTRAR NOTAS FISCAIS E RECIBOS");
                        System.out.println("===========================================");

                        System.out.println("Projetos cadastrados:");
                        ecomp.imprimirProjetos();

                        System.out.print("Digite o Id do projeto desejado: ");
                        int idProjeto = Integer.parseInt(input.nextLine());

                        Projeto projeto = ecomp.selecionarProjeto(idProjeto);
                        if (projeto == null) {
                            System.out.println("Projeto não encontrado!.");
                            break;
                        }

                        System.out.print("Data: ");
                        String data = input.nextLine();

                        System.out.print("Valor: ");
                        float valor = Float.parseFloat(input.nextLine());

                        System.out.print("Tipo: ");
                        String tipo = input.nextLine();

                        ecomp.cadastrarItemFiscal(data, valor, tipo, projeto);
                        System.out.println("\nItem Fiscal cadastrado com sucesso!");
                        break;
                    }
                case 6:
                {
                    System.out.println("===========================================");
                    System.out.println("            CADASTRAR ECOMPER");
                    System.out.println("===========================================");

                    System.out.print("Nome: ");
                    String nome = input.nextLine();

                    System.out.print("CPF: ");
                    String cpf = input.nextLine();

                    System.out.print("Email: ");
                    String email = input.nextLine();

                    System.out.print("Cargo: ");
                    String cargo = input.nextLine();

                    ecomp.cadastrarEcomper(nome, cpf, email, cargo);
                    System.out.println("\nEcomper cadastrado com sucesso!");
                    break;
                }
                case 7:
                    {
                        System.out.println("===========================================");
                        System.out.println("              GERAR RELATÓRIO");
                        System.out.println("===========================================");

                        System.out.println("Tipos disponíveis:");
                        System.out.println("1. Atividade");
                        System.out.println("2. Fiscal");
                        System.out.print("Escolha o tipo de relatório: ");

                        String tipoRelatorioInput = input.nextLine();
                        String tipoRelatorio;

                        if (tipoRelatorioInput.equals("1")) {
                            tipoRelatorio = "Atividade";
                        } else if (tipoRelatorioInput.equals("2")) {
                            tipoRelatorio = "Fiscal";
                        } else {
                            System.out.println("Tipo de relatório inválido!");
                            break;
                        }

                        Relatorio r = ecomp.gerarRelatorio(tipoRelatorio);
                        System.out.println("\nRelatório gerado com sucesso!");
                        ecomp.exibirRelatorio(r, tipoRelatorio);
                        break;
                    }
                case 8:
                    System.out.println("=========================================");
                    break;

                default:
                    System.out.println("Opção inválida! Escolha entre 1 e 8.");
                    break;
            }

        } while (op != 8);

        input.close();
    }
}
