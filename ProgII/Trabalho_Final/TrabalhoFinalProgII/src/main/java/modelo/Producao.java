package modelo;

import java.time.LocalDate;
import org.bson.codecs.pojo.annotations.BsonProperty;

public class Producao {
    
    @BsonProperty(value = "brinco")
    private String brinco;
    
    @BsonProperty(value = "data")
    private LocalDate data;
    
    @BsonProperty(value = "quantidade")
    private double quantidade;
    
    private transient String nome;
    
    public Producao(){
    }
    
    public Producao(String brinco, LocalDate data, double quantidade){
        this.brinco = brinco;
        this.data = data;
        this.quantidade = quantidade;
    }
    
    public String getBrinco(){
        return brinco;
    }
    public void setBrinco(String brinco){
        this.brinco = brinco;
    }
    
    public LocalDate getData(){
        return data;
    }
    public void setData(LocalDate data){
        this.data = data;
    }
    
    public double getQuantidade(){
        return quantidade;
    }
    public void setQuantidade(double quantidade){
        this.quantidade = quantidade;
    }
    
    public String getNome(){
        return nome;
    }
    public void setNome(String nome){
        this.nome = nome;
    }
}
