# Project Repository: Onion Classification with XGBoost
![Onion Classification Overview](assets/result.png)

## **Overview**

This repository contains code and resources for the classification of onions using hyperspectral image data. The project employs an XGBoost model for training and inference, leveraging preprocessed pixel data and segmentation techniques to differentiate between onion quality categories (e.g., "goed," "interne rot," "doorschot").

---

## **What is XGBoost?**

XGBoost (Extreme Gradient Boosting) is a powerful and efficient machine learning algorithm designed for structured data. It is based on the concept of boosting, where multiple weak learners (e.g., decision trees) are combined to create a strong model. Key characteristics of XGBoost include:

- **Boosting Algorithm**: Iteratively improves predictions by focusing on misclassified data.
- **High Performance**: Optimized for speed and accuracy, making it ideal for large datasets.
- **Regularization**: Built-in L1 and L2 regularization to prevent overfitting.
- **Feature Importance**: Identifies the most significant features for predictions.
- **Flexibility**: Supports classification, regression, and ranking tasks.

In this project, XGBoost is used to classify onion pixels into different quality categories. The model is trained on segmented and PLS-reduced pixel data, enabling robust predictions even with high-dimensional data.

---

## **Directory Structure**

```
.
├── .venv/               # Virtual environment directory
├── .vscode/             # VS Code settings
├── assets/              # Contains images for this file
├── data/                # Contains the dataset (hyperspectral images)
├── models/              # Trained models (e.g., XGBoost, PLS models)
├── notebooks/           # Jupyter notebooks for experimentation
├── scripts/             # Python scripts for reusable modules
├── train.ipynb          # Notebook for training the XGBoost model
├── inference.ipynb      # Notebook for loading and evaluating the trained model
├── README.md            # Project documentation (this file)
```

---

## **Setup Instructions**

### **1. Clone the Repository**

```bash
git clone https://github.com/Momin2004/Hyperspectral-imaging.git
cd src/python
```

### **2. Set Up the Virtual Environment**

```bash
python3 -m venv .venv
source .venv/bin/activate  # On Windows: .venv\Scripts\activate
```

### **3. Install Required Packages**

```bash
pip install -r requirements.txt
```

Alternatively, you can install the required packages individually:

```bash
pip install tifffile
pip install scikit-learn
pip install keras
pip install tensorflow
pip install matplotlib
pip install opencv-python
pip install xgboost
```

### **4. Dataset Preparation**

- The dataset can be found in the Teams channel 'General' within the datasets folder. It is provided as a ZIP file named `Definitieve Dataset v4`. This dataset contains all the labeled data.
- Additionally, a split ZIP version named `Definitieve Dataset v4 Gesplitst` is available, containing separate train and test folders. Extract the ZIP files and place them into the `data/` directory for further use.

---

## **Usage**

### **1. Training the Model**

Run the `train.ipynb` notebook to preprocess data, train the XGBoost model, and save it to the `models/` directory.

Steps:

- Load and preprocess the dataset (segmentation into pixel groups).
- Train the XGBoost model using PLS-reduced features.
- Save the Scaler Model to `models/scaler.joblib`.
- Save the LabelEncoder model to `models/label_encoder.joblib`.
- Save the PLS model to `models/pls_model.joblib`.
- Evaluate the model on test data.
- Save the trained model to `models/xgb_model.joblib`.

### **2. Inference and Evaluation**

Run the `inference.ipynb` notebook to:

- Load the trained XGBoost model. Pretrained models are also available in the Teams channel `General` under the `Models` folder as a ZIP file. Extract the models into the `models/` directory to use them directly.
- Test the model on new images.
- Evaluate performance (e.g., accuracy, dominant label classification).
- Visualize results, including pixel-wise predictions and reconstructed images.

---

## **Key Features**

- **Segmentation**: Differentiates between background pixels and onion pixels using binary masks.
- **Dimensionality Reduction**: Employs Partial Least Squares (PLS) for feature reduction.
- **Model Training**: Uses XGBoost for robust and efficient classification.
- **Evaluation Metrics**: Reports accuracy at both pixel and image levels.
- **Visualization**: Generates visual representations of predictions and segmentations.

---

## **Acknowledgments**

Special thanks to the team for their contributions to data preparation, model training, and evaluation.

---

## **Future Work**

- Expand the dataset for better generalization.
- Experiment with additional machine learning models or deep learning models.

---

## **Contact**

For questions or feedback, please contact:

- **Email**: a.okkema@student.avans.nl
- **GitHub**: [My GitHub Profile](https://github.com/AnoukOkkema)
