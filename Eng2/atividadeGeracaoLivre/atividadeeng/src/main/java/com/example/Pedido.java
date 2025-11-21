import java.util.Date;
import java.util.List;

public class Pedido {

    // Atributos baseados no Diagrama de Classe
    private int idPedido;
    private Usuario usuario;
    private Date dataPedido;
    private String status;
    private double valorTotal;
    private Endereco enderecoEntrega;
    private List<ItemCarrinho> itens;

    public Pedido(Usuario usuario, Endereco enderecoEntrega, List<ItemCarrinho> itens) {
        this.usuario = usuario;
        this.enderecoEntrega = enderecoEntrega;
        this.itens = itens;
        this.dataPedido = new Date();
        this.status = "Pendente";
    }

    // Método 1, passo 7
    // "Passo 7: O sistema calcula o valor total do pedido."
    public void calcularValorTotal() {
        System.out.println("Calculando valor total do pedido...");
        double total = 0.0;

        // lista de itens do pedido
        for (ItemCarrinho item : this.itens) {
            // subtotal de cada item foi calculado, apsso 3
            item.calcularSubtotal();

            // Soma o subtotal do item ao total geral do pedido
            total += item.getSubtotal();
        }

        // valor total do pedido
        this.valorTotal = total;
        System.out.println("Valor total calculado: R$ " + this.valorTotal);
    }

    // método 2, fluxo alternativo 7.a
    /*
     * "Cupom de Desconto
     * 1. O sistema calcula o valor do desconto.
     * 2. Retorna ao Fluxo Principal no passo 8."
     */
    public void aplicarCupomDesconto(Cupom cupom) {
        System.out.println("Aplicando cupom de desconto...");

        if (this.valorTotal == 0) {
            System.out.println("Valor total é zero, não é possível aplicar cupom.");
            return;
        }

        if (cupom.getTipo().equals("FIXO")) {
            // Desconto de valor fixo (ex: R$ 20,00)
            double valorAntigo = this.valorTotal;
            this.valorTotal -= cupom.getValor();
            System.out.println("Cupom FIXO de R$ " + cupom.getValor() + " aplicado.");
            System.out.println("Valor antigo: R$ " + valorAntigo + " -> Novo Valor: R$ " + this.valorTotal);
        } else if (cupom.getTipo().equals("PERCENTUAL")) {
            // Desconto percentual (ex: 15%)
            double valorAntigo = this.valorTotal;
            double valorDesconto = this.valorTotal * (cupom.getValor() / 100.0);
            this.valorTotal -= valorDesconto;
            System.out.println("Cupom PERCENTUAL de " + cupom.getValor() + "% aplicado (Desconto de R$ "
                    + String.format("%.2f", valorDesconto) + ").");
            System.out.println("Valor antigo: R$ " + String.format("%.2f", valorAntigo) + " -> Novo Valor: R$ "
                    + String.format("%.2f", this.valorTotal));
        }

        // Garante que o valor total não fique negativo após o desconto
        if (this.valorTotal < 0) {
            this.valorTotal = 0.0;
        }
    }

    // Getters para exibição
    public double getValorTotal() {
        return valorTotal;
    }

    public String getStatus() {
        return status;
    }
}