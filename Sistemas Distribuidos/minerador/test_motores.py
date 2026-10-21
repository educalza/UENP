import contextlib
import io
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch

import desempenho
from motores import Funcoes, MotorNativo, carregar_nativo, criar_motor, selecionar_motor
from protocolo import hash_bloco, preparar_hash_bytes


def motores_disponiveis():
    return ("python", "nativo", "nativo-evp") if selecionar_motor() != "python" else ("python",)


class TestMotores(unittest.TestCase):
    def test_nativo_mantem_evp_quando_via_direta_esta_ausente(self):
        if selecionar_motor() == "python":
            self.skipTest("Motor nativo não compilado.")
        biblioteca, crypto, funcoes = carregar_nativo()
        copia = Funcoes(*(getattr(funcoes, campo) for campo, _ in Funcoes._fields_[:6]),
                        None, None, None)
        trabalho = {"id": "evp", "dificuldade": 0, "dados": "ação"}
        with patch("motores.carregar_nativo", return_value=(biblioteca, crypto, copia)):
            motor = MotorNativo(trabalho, "Aluno")
            try:
                self.assertEqual(motor.nome, "nativo-evp")
                self.assertEqual(motor.buscar(10, 1, 2**64, 1)[2],
                                 (10, hash_bloco(trabalho, "Aluno", 10)))
            finally:
                motor.fechar()

    def test_cache_de_prefixos_longos_e_somas_decimais_no_mesmo_lote(self):
        # Prefixos cruzam blocos de 64 bytes; passos fazem várias propagações
        # de carry. A primeira solução de cada lote deve continuar idêntica.
        for escolha in motores_disponiveis():
            for nome in ("A" * 60, "☕" * 60):
                trabalho = {"dados": "x" * 200, "dificuldade": 2}
                motor = criar_motor(trabalho, nome, escolha)
                try:
                    for passo in (1, 3, 16, 9999, 2**63):
                        limite = (2**64 // passo) * passo
                        nonce = limite - passo
                        esperado = None
                        for indice in range(300):
                            digest = hash_bloco(trabalho, nome, nonce)
                            proximo = (nonce + passo) % limite
                            if digest.startswith("00"):
                                esperado = (nonce, digest)
                                break
                            nonce = proximo
                        with self.subTest(motor=escolha, nome=nome, passo=passo):
                            self.assertEqual(motor.buscar(limite - passo, passo, limite, 300),
                                             (proximo, indice + 1, esperado))
                finally:
                    motor.fechar()

    def test_hash_em_bytes_corresponde_ao_protocolo(self):
        trabalho = {"dificuldade": 0, "dados": "ação ☕ \n \\ \""}
        calcular = preparar_hash_bytes(trabalho, "Aluno")
        for nonce in (0, 9, 10, 999, 2**63, 2**64 - 1):
            self.assertEqual(calcular(nonce).hex(), hash_bloco(trabalho, "Aluno", nonce))

    def test_motores_preservam_json_e_transicoes_de_digitos(self):
        trabalho = {"id": 1, "dados": {"z": [True, None], "a": "ação ☕ \n \\ \""}, "dificuldade": 0}
        nome = 'Máquina "A" ☕'
        for escolha in motores_disponiveis():
            motor = criar_motor(trabalho, nome, escolha)
            try:
                for nonce in (0, 9, 10, 99, 100, 2**63, 2**64 - 1, 1):
                    with self.subTest(motor=escolha, nonce=nonce):
                        proximo, tentativas, solucao = motor.buscar(nonce, 1, 2**64, 1)
                        self.assertEqual(solucao, (nonce, hash_bloco(trabalho, nome, nonce)))
                        self.assertEqual(tentativas, 1)
                        self.assertEqual(proximo, (nonce + 1) % 2**64)
            finally:
                motor.fechar()

    def test_dificuldades_pares_impares_e_ausencia_de_solucao(self):
        for dificuldade in (0, 1, 2, 3, 4, 63, 64):
            trabalho = {"id": 1, "dados": "teste", "dificuldade": dificuldade}
            limite = 2**64 - 1
            nonce = limite - 9
            esperado = None
            tentativas = 400
            for indice in range(400):
                digest = hash_bloco(trabalho, "Aluno", nonce)
                proximo = (nonce + 3) % limite
                if digest.startswith("0" * dificuldade):
                    esperado = (nonce, digest)
                    tentativas = indice + 1
                    break
                nonce = proximo
            for escolha in motores_disponiveis():
                with self.subTest(dificuldade=dificuldade, motor=escolha):
                    motor = criar_motor(trabalho, "Aluno", escolha)
                    try:
                        obtido = motor.buscar(limite - 9, 3, limite, 400)
                        self.assertEqual(obtido, (proximo, tentativas, esperado))
                    finally:
                        motor.fechar()

    def test_motor_python_disponivel_quando_nativo_ausente(self):
        with patch("motores.MotorNativo", side_effect=OSError("não instalado")):
            self.assertEqual(selecionar_motor("auto"), "python")
            with self.assertRaisesRegex(RuntimeError, "compilar.py"):
                selecionar_motor("nativo")


class TestBenchmark(unittest.TestCase):
    def test_benchmark_cuda_usa_um_processo_e_preserva_cache_cpu(self):
        with tempfile.TemporaryDirectory() as pasta:
            cpu = Path(pasta) / "cpu.json"
            gpu = Path(pasta) / "gpu.json"
            cpu.write_text("preservar", encoding="utf-8")
            with patch.object(desempenho, "ARQUIVO", cpu), patch.object(
                desempenho, "ARQUIVO_CUDA", gpu
            ), patch("desempenho.identidade", return_value={"motor": "cuda"}), patch(
                "desempenho.medir", return_value=300_000_000
            ) as medir, contextlib.redirect_stdout(io.StringIO()):
                dados = desempenho.executar_benchmark(None, "cuda", 16)
                self.assertEqual(dados["processos"], 1)
                self.assertEqual(desempenho.carregar_configuracao("cuda"), 1)
                self.assertEqual(cpu.read_text(encoding="utf-8"), "preservar")
                medir.assert_called_once_with(None, 1, "cuda", 1.5)
                with self.assertRaises(ValueError):
                    desempenho.executar_benchmark(None, "cuda", 16, quantidade=2)

    def test_escolhe_melhor_taxa_e_invalida_cache_de_outro_codigo(self):
        with tempfile.TemporaryDirectory() as pasta:
            arquivo = Path(pasta) / "desempenho.json"
            with patch.object(desempenho, "ARQUIVO", arquivo), patch(
                "desempenho.medir", side_effect=(100, 300, 280, 260, 250)
            ), contextlib.redirect_stdout(io.StringIO()):
                resultado = desempenho.executar_benchmark(None, "python", 16)
                self.assertEqual(resultado["processos"], 4)
                self.assertEqual(desempenho.carregar_configuracao("python"), 4)
                with patch("desempenho.identidade", return_value={}):
                    self.assertIsNone(desempenho.carregar_configuracao("python"))
                arquivo.write_text("JSON incompleto", encoding="utf-8")
                self.assertIsNone(desempenho.carregar_configuracao("python"))

    def test_configuracao_explicita_mede_apenas_quantidade_pedida(self):
        with tempfile.TemporaryDirectory() as pasta:
            with patch.object(desempenho, "ARQUIVO", Path(pasta) / "desempenho.json"), patch(
                "desempenho.medir", return_value=100
            ) as medir, contextlib.redirect_stdout(io.StringIO()):
                resultado = desempenho.executar_benchmark(None, "python", 16, quantidade=3)
                self.assertEqual(resultado["processos"], 3)
                medir.assert_called_once_with(None, 3, "python", 1.5)


if __name__ == "__main__":
    unittest.main()
