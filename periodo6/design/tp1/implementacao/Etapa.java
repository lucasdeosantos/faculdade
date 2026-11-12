public class Etapa {
    private static int idCounter = 1;

    private int id;
    private String cronograma;
    private String status;

    public Etapa(String cronograma, String status) {
        this.id = idCounter++;
        this.cronograma = cronograma;
        this.status = status;
    }

    public int getId() { return id; }

    public String getCronograma() { return cronograma; }
    public String getStatus() { return status; }

    public void setCronograma(String cronograma) { this.cronograma = cronograma; }
    public void setStatus(String status) { this.status = status; }

    @Override
    public String toString() {
        return "Id: " + id + "\n"
            + "Cronograma: " + cronograma + "\n"
            + "Status: " + status;
    }
}
