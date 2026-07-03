# Low-Level Design Sandbox 🛠️ (`LLD_System_Prep_Cpp`)

Welcome to **LLD_System_Prep_Cpp**! This repository is a raw, self-driven playground for actively practicing and refining **Low-Level Design (LLD)** using Production-Grade C++. 

Instead of following a rigid, linear roadmap, this repo serves as an on-the-fly sandbox to implement, break, and optimize architectural patterns used in modern Infrastructure, Deep Learning runtimes, and distributed systems.

---

## 🎯 Active LLD Focus Areas

This repository focuses on designing clean, object-oriented, decoupled, and performant C++ systems for AI/ML workloads:

### 1. Architectural Patterns in Core ML
*   **Computational Graph Engines:** Designing Directed Acyclic Graphs (DAGs) representing neural network layers with clean forward/backward node execution tracking.
*   **Hardware Abstraction Layers:** Utilizing the **Factory Pattern** to cleanly swap execution backends (CPU, CUDA, Mock Hardware) without breaking higher-level layers.
*   **Configurable Token Samplers:** Implementing the **Strategy Pattern** to plug-and-play different inference generation logic (Top-k, Top-p, Temperature) dynamically.

### 2. Low-Level Resource Management & Memory Pools
*   **RAII Device Wrappers:** Writing safe C++ managers to handle underlying GPU/CUDA memory context lifetimes cleanly to guarantee zero VRAM leaks.
*   **Custom Memory Pools:** Designing low-latency block allocators tailored to mitigate fragmentation caused by volatile tensor allocations.
*   **Thread Pools for Inference:** Implementing thread-safe task queues to distribute massive tensor calculations efficiently across available hardware threads.

### 3. Object-Oriented Best Practices & Mechanics
*   **Const Correctness:** Eliminating unintended side-effects and enforcing compiler-level safety across complex object hierarchies.
*   **Move Semantics & Zero-Copy:** Maximizing throughput by transferring ownership of large metadata blocks (`std::move`) instead of triggering expensive allocations.
*   **Custom-Hashed Registry Collections:** Creating highly optimized tracking types leveraging custom `std::hash` specializations for lightning-fast container lookups.

---
