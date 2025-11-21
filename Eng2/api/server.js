const express = require('express');
const convert = require('color-convert');
const app = express();
const PORT = 3000;

// --- FUNÇÕES DE CONVERSÃO MANUAIS (ADICIONE NO INÍCIO DE server.js) ---
function hexToRgbManual(hex) {
    const processedHex = hex.length === 3
        ? hex.split('').map(char => char + char).join('')
        : hex;

    const r = parseInt(processedHex.substring(0, 2), 16);
    const g = parseInt(processedHex.substring(2, 4), 16);
    const b = parseInt(processedHex.substring(4, 6), 16);

    return [r, g, b];
}

function rgbToHexManual(r, g, b) {
    const toHex = (c) => c.toString(16).padStart(2, '0').toUpperCase();
    return `${toHex(r)}${toHex(g)}${toHex(b)}`;
}

// --- FIM DAS FUNÇÕES AUXILIARES ---

// --- Função Auxiliar para Enviar Respostas Padrão ---
const sendResponse = (res, success, message, data = null) => {
    return res.json({
        success,
        message,
        data
    });
};

// Middleware para validar o código HEX antes de qualquer rota
app.use((req, res, next) => {
    const hex = req.query.hex;

    // Métodos que exigem o parâmetro 'hex'
    const requiresHex = ['hex_para_rgb', 'calcular_complementar', 'gerar_paleta_triadica', 'obter_nome_cor'];

    if (requiresHex.includes(req.path.replace('/', '')) && !hex) {
        return sendResponse(res, false, 'O parâmetro "hex" é obrigatório (Ex: FF5733).');
    }

    if (hex) {
        const cleanHex = hex.toUpperCase().replace('#', '');

        // Validação básica se é HEX e tem o comprimento correto
        if (!/^[0-9A-F]{3}$|^[0-9A-F]{6}$/i.test(cleanHex)) {
            return sendResponse(res, false, 'O parâmetro "hex" deve ser um código de cor HEX válido (3 ou 6 dígitos).');
        }
        // Anexa o HEX limpo e capitalizado ao objeto request para uso posterior
        req.cleanHex = cleanHex;
    }
    next();
});

// ===================================================
// --- IMPLEMENTAÇÃO DOS 04 MÉTODOS DA API ---
// ===================================================

/**
 * 1. Converte um código HEX para RGB.
 * Rota: /hex_para_rgb?hex=FF5733
 */
app.get('/hex_para_rgb', (req, res) => {
    try {
        const hex = req.cleanHex;

        // 1. Garante 6 dígitos se for 3 (ex: F00 -> FF0000)
        let processedHex = hex.length === 3
            ? hex.split('').map(char => char + char).join('')
            : hex;

        // 2. Converte os pares de dígitos para decimal (base 16)
        const r = parseInt(processedHex.substring(0, 2), 16);
        const g = parseInt(processedHex.substring(2, 4), 16);
        const b = parseInt(processedHex.substring(4, 6), 16);

        const rgb = [r, g, b];
        const rgbString = `${r}, ${g}, ${b}`;

        sendResponse(res, true, 'Conversão de HEX para RGB realizada.', {
            hex: `#${hex}`,
            rgb: rgbString,
            rgb_array: rgb
        });
    } catch (error) {
        // Agora, se houver erro, a mensagem será mais específica
        console.error("Erro manual de conversão:", error.message);
        sendResponse(res, false, 'Erro na conversão HEX para RGB (verifique logs).', null);
    }
});

/**
 * 2. Calcula a cor complementar (inversa) de um código.
 * Rota: /calcular_complementar?hex=FF0000
 */
app.get('/calcular_complementar', (req, res) => {
    try {
        const [r, g, b] = hexToRgbManual(req.cleanHex);

        // Cálculo do RGB Complementar: Subtrai cada componente de 255
        const complementarR = 255 - r;
        const complementarG = 255 - g;
        const complementarB = 255 - b;

        const complementarHex = rgbToHexManual(complementarR, complementarG, complementarB);

        sendResponse(res, true, 'Cor complementar calculada (usando RGB inverso).', {
            original_hex: `#${req.cleanHex}`,
            complementar_hex: `#${complementarHex}`,
            complementar_rgb: `${complementarR}, ${complementarG}, ${complementarB}`
        });
    } catch (error) {
        console.error("Erro manual ao calcular complementar:", error.message);
        sendResponse(res, false, 'Erro ao calcular a cor complementar.', null);
    }
});

/**
 * Funções auxiliares (coloque-as fora da rota para reutilizar)
 * NOTA: Estas são implementações simples de HSL/RGB para demonstração.
 */

// NOTA: Para este método funcionar, garanta que as funções hexToRgbManual e rgbToHexManual
// estejam definidas e disponíveis para esta rota.

/**
 * Converte RGB (Array) para HSL (Array) - APENAS PARA OBTER O MATIZ (H)
 * Fonte: Baseado em algoritmos de conversão padrão.
 */
function rgbToHslManual(r, g, b) {
    r /= 255;
    g /= 255;
    b /= 255;

    const max = Math.max(r, g, b);
    const min = Math.min(r, g, b);
    let h, s, l = (max + min) / 2;

    if (max === min) {
        h = s = 0; // Acinzentado
    } else {
        const d = max - min;
        s = l > 0.5 ? d / (2 - max - min) : d / (max + min);
        switch (max) {
            case r: h = (g - b) / d + (g < b ? 6 : 0); break;
            case g: h = (b - r) / d + 2; break;
            case b: h = (r - g) / d + 4; break;
        }
        h /= 6;
    }
    // Retorna Hue em graus (0-360), Saturation (0-100), Lightness (0-100)
    return [Math.round(h * 360), Math.round(s * 100), Math.round(l * 100)];
}

/**
 * Converte HSL (Array) para RGB (Array)
 * Fonte: Baseado em algoritmos de conversão padrão.
 */
function hslToRgbManual(h, s, l) {
    h /= 360;
    s /= 100;
    l /= 100;
    let r, g, b;

    if (s === 0) {
        r = g = b = l; // Acinzentado
    } else {
        const hue2rgb = (p, q, t) => {
            if (t < 0) t += 1;
            if (t > 1) t -= 1;
            if (t < 1 / 6) return p + (q - p) * 6 * t;
            if (t < 1 / 2) return q;
            if (t < 2 / 3) return p + (q - p) * (2 / 3 - t) * 6;
            return p;
        };

        const q = l < 0.5 ? l * (1 + s) : l + s - l * s;
        const p = 2 * l - q;
        r = hue2rgb(p, q, h + 1 / 3);
        g = hue2rgb(p, q, h);
        b = hue2rgb(p, q, h - 1 / 3);
    }

    return [Math.round(r * 255), Math.round(g * 255), Math.round(b * 255)];
}


// --- CÓDIGO DA ROTA /gerar_paleta_triadica ---

app.get('/gerar_paleta_triadica', (req, res) => {
    try {
        const [r, g, b] = hexToRgbManual(req.cleanHex);

        // 1. Converte RGB para HSL
        let [h, s, l] = rgbToHslManual(r, g, b);

        // 2. Calcula as rotações de Matiz (Hue) para a tríade (H, H+120, H+240)
        const h1 = (h + 120) % 360;
        const h2 = (h + 240) % 360;

        // 3. Converte os novos HSLs de volta para RGB
        const [r1, g1, b1] = hslToRgbManual(h1, s, l);
        const [r2, g2, b2] = hslToRgbManual(h2, s, l);

        // 4. Converte RGB de volta para HEX
        const corBase = `#${req.cleanHex.toUpperCase()}`;
        const cor1 = `#${rgbToHexManual(r1, g1, b1)}`;
        const cor2 = `#${rgbToHexManual(r2, g2, b2)}`;

        sendResponse(res, true, 'Paleta Triádica gerada (usando HSL manual).', {
            base_hex: corBase,
            paleta: [corBase, cor1, cor2]
        });
    } catch (error) {
        console.error("Erro manual ao gerar paleta:", error.message);
        sendResponse(res, false, 'Erro ao gerar a paleta triádica.', null);
    }
});

/**
 * 4. Obtém um nome de cor descritivo a partir do código HEX.
 * Rota: /obter_nome_cor?hex=4682B4
 */
app.get('/obter_nome_cor', (req, res) => {
    // Lista de cores predefinidas (muito pequena, apenas para demonstração)
    const colorNames = {
        'FF0000': 'Vermelho Puro',
        '00FF00': 'Verde Limão',
        '0000FF': 'Azul Puro',
        '4682B4': 'Azul Aço (Steel Blue)',
        'FFD700': 'Ouro'
    };

    if (colorNames[req.cleanHex]) {
        sendResponse(res, true, 'Nome de cor encontrado na lista.', {
            hex: `#${req.cleanHex}`,
            name: colorNames[req.cleanHex]
        });
    } else {
        sendResponse(res, false, 'Nenhum nome descritivo encontrado para este HEX na lista interna.', {
            hex: `#${req.cleanHex}`,
            tip: 'Tente um dos códigos conhecidos: FF0000, 4682B4, FFD700.'
        });
    }
});

// Adicione esta nova rota ao seu arquivo server.js

app.get('/docs', (req, res) => {
    return res.json({
        "mensagem": "API de Análise e Paletas de Cores (Node.js)",
        "descricao": "Esta API fornece utilitários para conversão e manipulação de códigos de cores HEX.",
        "endpoints": [
            {
                "rota": "/hex_para_rgb",
                "metodo": "GET",
                "parametros_necessarios": "hex (código de cor HEX de 3 ou 6 dígitos)",
                "exemplo_url": "http://localhost:3000/hex_para_rgb?hex=1E90FF",
                "descricao": "Converte um código de cor HEX para seu equivalente RGB."
            },
            {
                "rota": "/calcular_complementar",
                "metodo": "GET",
                "parametros_necessarios": "hex (código de cor HEX de 3 ou 6 dígitos)",
                "exemplo_url": "http://localhost:3000/calcular_complementar?hex=FF0000",
                "descricao": "Calcula a cor complementar (inversa) de um código HEX."
            },
            {
                "rota": "/gerar_paleta_triadica",
                "metodo": "GET",
                "parametros_necessarios": "hex (código de cor HEX de 3 ou 6 dígitos)",
                "exemplo_url": "http://localhost:3000/gerar_paleta_triadica?hex=00FF00",
                "descricao": "Gera uma paleta de 3 cores (triádica) baseada no código HEX fornecido, rotacionando o matiz."
            },
            {
                "rota": "/obter_nome_cor",
                "metodo": "GET",
                "parametros_necessarios": "hex (código de cor HEX de 3 ou 6 dígitos)",
                "exemplo_url": "http://localhost:3000/obter_nome_cor?hex=4682B4",
                "descricao": "Busca um nome descritivo para o código HEX fornecido em uma lista interna (limitada)."
            }
        ],
        "observacao": "Todos os códigos HEX podem ser passados com ou sem o caractere '#'."
    });
});

// Rota padrão (Erro ou Ajuda)
app.use((req, res) => {
    const methods = ['hex_para_rgb', 'calcular_complementar', 'gerar_paleta_triadica', 'obter_nome_cor'];
    sendResponse(res, false, 'Método inválido ou não especificado.', {
        available_methods: methods,
        example_usage: `http://localhost:${PORT}/hex_para_rgb?hex=1E90FF`
    });
});

// Inicia o servidor
app.listen(PORT, () => {
    console.log(`API de Cores rodando em http://localhost:${PORT}`);
});