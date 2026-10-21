"""Testes executados na GPU real; ignorados se CUDA/NVRTC não estiver disponível."""
import unittest

from cuda_minerador import MotorCUDA, fechar_sessao, informacoes_cuda
from protocolo import hash_bloco


class TestCUDA(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        try:
            cls.info = informacoes_cuda()
        except (OSError, RuntimeError) as erro:
            raise unittest.SkipTest(str(erro))

    @classmethod
    def tearDownClass(cls):
        fechar_sessao()

    def test_hashes_gpu_com_unicode_e_limites_de_uint64(self):
        trabalho = {"id": "teste", "dados": {"texto": "ação ☕ \n \\ \""}, "dificuldade": 0}
        for nome in ("Eduardo", "☕" * 60):
            motor = MotorCUDA(trabalho, nome)
            try:
                for nonce in (0, 9, 10, 99, 100, 2**63, 2**64 - 1):
                    with self.subTest(nome=nome, nonce=nonce):
                        proximo, tentativas, solucao = motor.buscar(nonce, 1, 2**64, 1)
                        self.assertEqual(tentativas, 1)
                        self.assertEqual(proximo, (nonce+1) % 2**64)
                        self.assertEqual(solucao, (nonce, hash_bloco(trabalho, nome, nonce)))
            finally:
                motor.fechar()

    def test_primeira_solucao_e_contagem_de_trabalho_paralelo(self):
        for dificuldade in (1, 2, 3, 4, 63, 64):
            trabalho = {"id": "lote", "dados": "teste", "dificuldade": dificuldade}
            motor = MotorCUDA(trabalho, "Aluno")
            try:
                inicio, passo, limite, lote = 9990, 3, 2**64 - 1, 4096
                esperado = None
                nonce = inicio
                for indice in range(lote):
                    digest = hash_bloco(trabalho, "Aluno", nonce)
                    if digest.startswith("0" * dificuldade):
                        esperado = (nonce, digest)
                        break
                    nonce = (nonce + passo) % limite
                proximo, tentativas, solucao = motor.buscar(inicio, passo, limite, lote)
                with self.subTest(dificuldade=dificuldade):
                    self.assertEqual(solucao, esperado)
                    self.assertLessEqual(tentativas, lote)
                    self.assertEqual(proximo, (inicio + tentativas * passo) % limite)
                    if esperado is None:
                        self.assertEqual(tentativas, lote)
                    else:
                        self.assertGreaterEqual(tentativas, indice+1)
            finally:
                motor.fechar()

    def test_wrap_sem_repetir_ou_perder_tentativas(self):
        trabalho = {"id": "wrap", "dados": "x", "dificuldade": 64}
        motor = MotorCUDA(trabalho, "Aluno")
        try:
            for passo in (1, 3, 16, 2**63):
                limite = (2**64 // passo) * passo
                inicio = limite - passo
                with self.subTest(passo=passo):
                    proximo, tentativas, solucao = motor.buscar(inicio, passo, limite, 8)
                    self.assertEqual((proximo, tentativas, solucao),
                                     ((inicio+8*passo) % limite, 8, None))
        finally:
            motor.fechar()

    def test_gpu_atende_troca_de_rodada_e_encerra_processo(self):
        from minerador import MineracaoParalela
        from test_minerador import TestProcessos
        pool = MineracaoParalela(1, motor="cuda")
        trabalho = {"id": 1, "altura": 1, "dados": "teste", "dificuldade": 64}
        novo = dict(trabalho, id=2, dificuldade=0)
        try:
            pool.iniciar(trabalho, "Aluno")
            pool.parar()
            pool.iniciar(novo, "Aluno")
            nonce, digest = TestProcessos().aguardar(pool, 1)[0]
            self.assertEqual(digest, hash_bloco(novo, "Aluno", nonce))
        finally:
            pool.fechar()
        self.assertTrue(all(not p.is_alive() for p in pool.processos))

    def test_gpu_rejeita_processos_duplicados(self):
        from minerador import MineracaoParalela
        with self.assertRaisesRegex(ValueError, "um processo"):
            MineracaoParalela(2, motor="cuda")

    def test_http_com_prova_calculada_na_rtx(self):
        from test_minerador import TestIntegracao
        # Reutiliza o servidor HTTP simulado e confere a prova na CPU.
        TestIntegracao().exercitar_http("cuda")


if __name__ == "__main__":
    unittest.main()
