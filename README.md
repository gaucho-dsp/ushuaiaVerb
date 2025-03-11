# UshuaiaVerb: An OS Algorithmic Reverb using Householder Matrices
![poster ushaiaVerb cortado](https://github.com/user-attachments/assets/88c0ea0d-36bb-4305-9f90-edf3b8776978)

### Overview
UshuaiaVerb is an algorithmic reverb designed using cascaded delay networks with structured mixing via a **5×5 Householder matrix**. The architecture prioritizes high echo density, efficient diffusion, and stereo decorrelation through asymmetric delay paths.

### Goals
- **Energy-preserving diffusion** using Householder reflections.
- **Asymmetric delay feedback** for stereo decorrelation.
- **Time-domain processing** with minimal computational overhead.
- **Smooth decay characteristics** without explicit filtering.

### Mathematical Details
- Echo density increases **quadratically** with time.
- The feedback matrix remains **unitary**, for stable decay.
- Processing complexity scales as **O(N + L log L)** with respect to delay lengths.

### Poster
For a more detailed explanation, refer to the **[technical poster](Poster_for_UshuaiaVerb.pdf)**.



---
