import java.util.ArrayList;
import java.util.List;

public class Relatorio {
    private String tipo;
    private List<Atividade> atividades;
    private List<ItemFiscal> itensFiscais;

    public Relatorio(String tipo) {
        this.tipo = tipo;
        this.atividades = new ArrayList<>();
        this.itensFiscais = new ArrayList<>();
    }

    public String getTipo() { return tipo; }
    
    public void setTipo(String tipo) { this.tipo = tipo; }

    public void gerarRelatorioAtividade(List<Atividade> atividades) {
        System.out.println("Gerando relatório de atividades...");

        for (Atividade a : atividades) {
            adicionarAtividadeRelatorio(a);
        }
    }

    public void adicionarAtividadeRelatorio(Atividade a) {
        atividades.add(a);
    }

    public void visualizarRelatorioAtividades() {
        for (Atividade a : atividades) {
            System.out.println(a);
            System.out.println();
        }
    }

    public void gerarRelatorioItensFiscais(List<Projeto> projetos) {
        System.out.println("Gerando relatório de itens fiscais...");

        for (Projeto p : projetos) {
            for (ItemFiscal item : p.getItensFiscais()) {
                adicionarItemFiscalRelatorio(item);
            }
        }
    }

    public void adicionarItemFiscalRelatorio(ItemFiscal item) {
        itensFiscais.add(item);
    }

    public void visualizarRelatorioItensFiscais() {
        for (ItemFiscal item : itensFiscais) {
            System.out.println(item);
            System.out.println();
        }
    }
}
