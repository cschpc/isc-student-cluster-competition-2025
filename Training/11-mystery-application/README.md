# SCC Training 11 - Practicalities and mystery application (at CSC)

Date 5.6.2025  
Slides: [https://kannu.csc.fi/s/cCdRYPo5q23qa2a](https://kannu.csc.fi/s/cCdRYPo5q23qa2a)

## Agenda

|Time|Topic|
|---|---|
|15:30|Discussion on previous topics|
|15:45|Practicalities|
|16:15|Mystery application|
|18:00|Training concluded|

Mystery application:

## Step-by-Step: Set Up Qiskit + JupyterLab in a Virtual Environment

### 1. Create and activate the virtual environment

```bash
# Create the virtual environment
python3 -m venv qiskit-env

# Activate it
# On macOS/Linux:
source qiskit-env/bin/activate

# On Windows (Command Prompt):
qiskit-env\Scripts\activate

# On Windows (PowerShell):
.\qiskit-env\Scripts\Activate.ps1
```

---

### 2. Upgrade tools and install all needed packages

```bash
# Upgrade pip and core tooling
pip install --upgrade pip setuptools wheel

# Install Qiskit with Aer simulator support
pip install qiskit[qiskit-aer]

# Install visualization support
pip install matplotlib pylatexenc

# Install JupyterLab
pip install jupyterlab
```

---

### 3. Add the venv to Jupyter kernels

This allows you to select this venv as a kernel inside JupyterLab:

```bash
pip install ipykernel
python -m ipykernel install --user --name=qiskit-env --display-name "Python (qiskit-env)"
```

---

### 4. Launch JupyterLab

```bash
jupyter lab
```

Your browser will open with the JupyterLab interface. When opening a notebook, choose the kernel called:

```
Python (qiskit-env)
```
