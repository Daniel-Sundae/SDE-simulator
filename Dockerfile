# Stage 1: build
FROM ubuntu:22.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

# Update package lists first
RUN apt-get update

# Base dependencies, less likely to change
RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates \
    gnupg \
    git \
    && rm -rf /var/lib/apt/lists/*

# Build tools, might change occasionally
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    pkg-config \
    && rm -rf /var/lib/apt/lists/*

# Graphics/Image libraries
RUN apt-get update && apt-get install -y --no-install-recommends \
    libpng-dev \
    libjpeg-turbo8-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    libxkbcommon-dev \
    libvulkan-dev \
    && rm -rf /var/lib/apt/lists/*

# Qt dependencies
RUN apt-get update && apt-get install -y --no-install-recommends \
    qt6-base-dev \
    libqt6charts6-dev \
    qt6-tools-dev \
    libqt6opengl6-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src
COPY . /src

# Build the application
RUN mkdir -p build && cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    cmake --build . -- -j$(nproc)

# Stage 2: runtime
FROM ubuntu:22.04 AS runtime

# Update package lists first
RUN apt-get update

# Install runtime dependencies
RUN apt-get install -y --no-install-recommends \
    libpng16-16 \
    libjpeg-turbo8 \
    libqt6core6 \
    libqt6gui6 \
    libqt6widgets6 \
    libqt6charts6 \
  # Clean up apt cache in the same layer as install
  && rm -rf /var/lib/apt/lists/*

WORKDIR /app
# Copy only the built executable from the builder stage
COPY --from=builder /src/build/StochasticSim /app/

ENTRYPOINT ["./StochasticSim"]
CMD []
