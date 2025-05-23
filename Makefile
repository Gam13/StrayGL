# Configurações básicas
BUILD_DIR := build
VCPKG_DIR := $(BUILD_DIR)/vcpkg
VCPKG_INSTALL_DIR := $(BUILD_DIR)/vcpkg_installed

# Detecção automática do triplet válido
UNAME_M := $(shell uname -m)
UNAME_S := $(shell uname -s | tr '[:upper:]' '[:lower:]')

ifeq ($(UNAME_S),linux)
    TRIPLET := x64-linux  # Sempre usa x64-linux, mesmo em arquiteturas diferentes
else ifeq ($(UNAME_S),darwin)
    TRIPLET := x64-osx
else ifeq ($(OS),Windows_NT)
    TRIPLET := x64-windows
else
    $(error "Sistema operacional não suportado: $(UNAME_S)")
endif

.PHONY: init_vcpkg deps configure

init_vcpkg:
	@mkdir -p $(BUILD_DIR)
	@if [ ! -d "$(VCPKG_DIR)/.git" ]; then \
		echo "🧩 Clonando vcpkg..."; \
		git clone https://github.com/microsoft/vcpkg.git $(VCPKG_DIR); \
		echo "🛠 Configurando vcpkg..."; \
		$(VCPKG_DIR)/bootstrap-vcpkg$(if $(filter Windows_NT,$(OS)),.bat,.sh); \
	fi

deps: init_vcpkg
	@echo "📦 Instalando dependências para $(TRIPLET)..."; \
	$(VCPKG_DIR)/vcpkg$(if $(filter Windows_NT,$(OS)),.exe) install --triplet $(TRIPLET) --x-install-root=$(VCPKG_INSTALL_DIR)

configure:
	@$(MAKE) deps