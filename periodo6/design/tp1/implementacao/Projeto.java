import java.util.ArrayList;
import java.util.List;

public class Projeto {
    private static int idCounter = 1;

    private int id;
    private Cliente cliente;
    private String dataInicio;
    private String prazo;
    private String descricao;
    private String contrato;

    private List<Desenvolvedor> devs;
    private List<ItemFiscal> itensFiscais;
    private List<Etapa> etapasProjeto;

    public Projeto(Cliente cliente, String dataInicio, String prazo, String descricao, String contrato) {
        this.id = idCounter++;
        this.cliente = cliente;
        this.dataInicio = dataInicio;
        this.prazo = prazo;
        this.descricao = descricao;
        this.contrato = contrato;

        this.devs = new ArrayList<>();
        this.itensFiscais = new ArrayList<>();
        this.etapasProjeto = new ArrayList<>();
    }

    public int getId() { return id; }
    public List<ItemFiscal> getItensFiscais() { return itensFiscais; }

    public Cliente getCliente() { return cliente; }
    public String getDataInicio() { return dataInicio; }
    public String getPrazo() { return prazo; }
    public String getDescricao() { return descricao; }
    public String getContrato() { return contrato; }

    public void setCliente(Cliente cliente) { this.cliente = cliente; }
    public void setDataInicio(String dataInicio) { this.dataInicio = dataInicio; }
    public void setPrazo(String prazo) { this.prazo = prazo; }
    public void setDescricao(String descricao) { this.descricao = descricao; }
    public void setContrato(String contrato) { this.contrato = contrato; }

    public boolean cadastrarDesenvolvedor(Desenvolvedor dev) {
        for (Desenvolvedor d : devs) {
            if (d.getId() == dev.getId()) {
                return false;
            }
        }

        devs.add(dev);
        return true;
    }

    public void adicionarItemFiscal(ItemFiscal item) {
        itensFiscais.add(item);
    }

    public Etapa cadastrarEtapas(String cronograma, String statusProjeto) {
        Etapa etapa = new Etapa(cronograma, statusProjeto);
        adicionarEtapa(etapa);
        return etapa;
    }

    public void adicionarEtapa(Etapa etapa) {
        etapasProjeto.add(etapa);
    }

    @Override
    public String toString() {
        return "Id: " + id + "\n" 
            + "Data Inicio: " + dataInicio + "\n" 
            + "Prazo: " + prazo + "\n" 
            + "Descrição: " + descricao + "\n" 
            + "Contrato: " + contrato + "\n" 
            + "Desenvolvedores: " + devs + "\n" 
            + "Itens Fiscais: " + itensFiscais + "\n" 
            + "Etapas: " + etapasProjeto + "\n"
            + "Cliente:\n" + cliente; 
    }
}
