# **StrayGL** - Projeto de Aprendizado em OpenGL

![OpenGL Logo](https://www.opengl.org/img/opengl_logo.png)  
*Um projeto educacional para explorar gráficos 2D e 3D com OpenGL, GLFW e GLAD*

## 📌 **Visão Geral**
Este projeto é uma biblioteca/ambiente de aprendizado para OpenGL moderno (core profile), criado com:
- **GLFW** para gerenciamento de janelas e inputs
- **GLAD** para carregamento de funções OpenGL
- **Dear ImGui** para interfaces de debug
- **CMake** para construção multiplataforma

## 🎯 **Objetivos de Aprendizado**
- Fundamentos de renderização 2D e 3D com OpenGL
- Pipeline gráfico moderno (shaders, VBOs, VAOs)
- Gerenciamento de recursos gráficos
- Técnicas básicas de iluminação e câmera
- Boas práticas de organização de projetos gráficos

## ⚙️ **Dependências**
| Biblioteca | Versão | Propósito |
|------------|--------|-----------|
| OpenGL     | 4.6+   | API gráfica |
| GLFW       | 3.3+   | Criação de janelas/inputs |
| GLAD       | -      | Loader de funções OpenGL |
| Dear ImGui | 1.89+  | UI para debug |

## 🛠️ **Compilação**
```bash
# Clone o repositório
git clone https://github.com/Gam13/StrayGL.git
cd StrayGL

# Configure (Linux/macOS)
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release

# Compile
make -j4

# Windows (MSVC)
cmake -B build -A x64
cmake --build build --config Release
```

## 📂 **Estrutura do Projeto
```
StrayGL/
├── 3thparty/      # Dependências (GLFW, GLAD, ImGui)
├── include/       # Headers públicos
├── src/           # Implementação
├── examples/       # Exemplos de uso
├── CMakeLists.txt # Configuração de build
└── README.md
```


## 🌟 **Recursos Implementados**
    ✅ - Logger
    ✅ - Criação de Janela

## 📚 **Recursos de Aprendizado**
- [LearnOpenGL](https://learnopengl.com/)
- [OpenGL Reference Cards](https://www.khronos.org/opengl/wiki/Main_Page)
- [OpenGL Documentation](https://docs.gl/)
- [GLFW Documentation](https://www.glfw.org/docs/latest/)

## 🤝 **Como Contribuir**
1. Faça um fork do projeto
2. Crie uma branch (`git checkout -b feature/nova-funcionalidade`)
3. Commit suas mudanças (`git commit -m 'Adiciona X feature'`)
4. Push para a branch (`git push origin feature/nova-funcionalidade`)
5. Abra um Pull Request

## 📄 **Licença**
MIT License - Veja o arquivo [LICENSE](LICENSE) para detalhes

