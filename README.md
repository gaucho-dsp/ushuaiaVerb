# UshuaiaVerb: An OS Algorithmic Reverb using Householder Matrices
<img width="1033" height="576" alt="Screenshot 2025-12-12 at 19 13 00" src="https://github.com/user-attachments/assets/15e40c2f-ce04-4d58-a7bd-86c02190218d" />


### Overview
UshuaiaVerb is an algorithmic reverb designed using cascaded delay networks with structured mixing via a **5×5 Householder matrix**. The architecture prioritises high echo density, efficient diffusion, and stereo decorrelation through asymmetric delay paths.


### Core Architecture
<img width="755" height="284" alt="ushuaiaVerb_BD" src="https://github.com/user-attachments/assets/d0b007f9-7693-4852-90e1-07eb50df6850" />

* **Terminal Diffusion Matrix**: A **5x5 recursive feedback lattice** that maximises echo density without metallic ringing.
* **Golden Ratio Reflections**: A distinct **4x4 early reflection stage** utilising $\phi$ (0.618...) coefficients for non-resonant diffusion.
* **Singular Allpass Modulation**: Pitch modulation is applied to a single delay node within the tank, creating subtle motion without inducing 'seasick' pitch drift.
* **Pearson Damping**: Multi-stage slew limiting inside the feedback loop simulates atmospheric high-frequency absorption.

### Signal Conditioning
* **SubTight & SubBoost**: Non-linear processing at the input and output stages tightens low-frequency transients, preventing the 'mud' typical of large cathedral algorithms.
* **Stereo Decorrelation**: Asymmetric delay lengths and offset vibrato LFOs ensure a wide, immersive stereo field.

### Acknowledgements
**Chris Johnson (Airwindows)** for the brilliant **kCathedral2** algorithm. 
