import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import java.util.ArrayList;
import java.util.List;

// Importa a função estática assertEquals para asserts mais limpos
import static org.junit.jupiter.api.Assertions.assertEquals;

class PedidoTest {

    // 1. Dependências que serão usadas em múltiplos testes
    private Usuario usuarioPadrao;
    private Endereco enderecoPadrao;
    private Produto produto1;
    private Produto produto2;

    @BeforeEach
    void setUp() {
        // Inicializa objetos base antes de CADA teste
        usuarioPadrao = new Usuario(); // Objeto mock/stub
        enderecoPadrao = new Endereco(); // Objeto mock/stub
        
        produto1 = new Produto(1, "Ração Premier", 150.00);
        produto2 = new Produto(2, "Brinquedo Ossinho", 25.50);
    }

    // --- Testes para o Método: calcularValorTotal() ---
    // Complexidade Ciclomática (M=2)

    @Test
    @DisplayName("UC7 (Passo 7) - Caso de Teste 1 (M=2): Lista Vazia")
    void testCalcularValorTotalComListaVazia() {
        // --- Cenário (Arrange) ---
        // Caminho 1: Loop 'for' é pulado (lista vazia)
        List<ItemCarrinho> itensVazios = new ArrayList<>();
        Pedido pedido = new Pedido(usuarioPadrao, enderecoPadrao, itensVazios);

        // --- Ação (Act) ---
        pedido.calcularValorTotal();

        // --- Verificação (Assert) ---
        // Espera-se 0.0, com uma margem de erro (delta) de 0.001 para doubles
        assertEquals(0.0, pedido.getValorTotal(), 0.001);
    }

    @Test
    @DisplayName("UC7 (Passo 7) - Caso de Teste 2 (M=2): Lista com Itens")
    void testCalcularValorTotalComItens() {
        // --- Cenário (Arrange) ---
        // Caminho 2: Loop 'for' executa
        List<ItemCarrinho> itens = List.of(
            new ItemCarrinho(produto1, 1), // 1x 150.00 = 150.00
            new ItemCarrinho(produto2, 2)  // 2x  25.50 =  51.00
        );
        Pedido pedido = new Pedido(usuarioPadrao, enderecoPadrao, itens);
        
        // Valor total esperado = 150.00 + 51.00 = 201.00
        double valorEsperado = 201.00;

        // --- Ação (Act) ---
        pedido.calcularValorTotal();

        // --- Verificação (Assert) ---
        assertEquals(valorEsperado, pedido.getValorTotal(), 0.001);
    }

    // --- Testes para o Método: aplicarCupomDesconto() ---
    // (Este método tem M=3)

    @Test
    @DisplayName("UC7 (Fluxo 7.a) - Cupom Fixo")
    void testAplicarCupomDescontoFixo() {
        // --- Cenário (Arrange) ---
        // Configura um pedido com valor total já calculado
        List<ItemCarrinho> itens = List.of(new ItemCarrinho(produto1, 1)); // Total = 150.00
        Pedido pedido = new Pedido(usuarioPadrao, enderecoPadrao, itens);
        pedido.calcularValorTotal(); // Total inicial é 150.00
        
        Cupom cupomFixo = new Cupom("FIXO20", "FIXO", 20.0);
        double valorEsperado = 130.00; // 150.00 - 20.00

        // --- Ação (Act) ---
        pedido.aplicarCupomDesconto(cupomFixo);

        // --- Verificação (Assert) ---
        assertEquals(valorEsperado, pedido.getValorTotal(), 0.001);
    }
    
    @Test
    @DisplayName("UC7 (Fluxo 7.a) - Cupom Percentual")
    void testAplicarCupomDescontoPercentual() {
        // --- Cenário (Arrange) ---
        // Usamos o mesmo pedido com 2 itens (Total = 201.00)
        List<ItemCarrinho> itens = List.of(
            new ItemCarrinho(produto1, 1), // 150.00
            new ItemCarrinho(produto2, 2)  //  51.00
        );
        Pedido pedido = new Pedido(usuarioPadrao, enderecoPadrao, itens);
        pedido.calcularValorTotal(); // Total inicial é 201.00

        Cupom cupomPercentual = new Cupom("NATAL10", "PERCENTUAL", 10.0); // 10% OFF
        
        // 201.00 * (10 / 100) = 20.10
        // 201.00 - 20.10 = 180.90
        double valorEsperado = 180.90;

        // --- Ação (Act) ---
        pedido.aplicarCupomDesconto(cupomPercentual);

        // --- Verificação (Assert) ---
        assertEquals(valorEsperado, pedido.getValorTotal(), 0.001);
    }
    
    @Test
    @DisplayName("UC7 (Fluxo 7.a) - Cupom Fixo maior que o valor total (trava em zero)")
    void testAplicarCupomQueZeraValor() {
        // --- Cenário (Arrange) ---
        List<ItemCarrinho> itens = List.of(new ItemCarrinho(produto2, 1)); // Total = 25.50
        Pedido pedido = new Pedido(usuarioPadrao, enderecoPadrao, itens);
        pedido.calcularValorTotal(); // Total inicial é 25.50

        Cupom cupomMuitoAlto = new Cupom("SUPEROFF", "FIXO", 100.0);
        double valorEsperado = 0.0; // Não deve ficar negativo

        // --- Ação (Act) ---
        pedido.aplicarCupomDesconto(cupomMuitoAlto);

        // --- Verificação (Assert) ---
        assertEquals(valorEsperado, pedido.getValorTotal(), 0.001);
    }
}