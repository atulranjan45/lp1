# Step 1: Import necessary libraries
import numpy as np
import tensorflow as tf
from tensorflow.keras.datasets import imdb
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense, Embedding, GlobalAveragePooling1D
from tensorflow.keras.preprocessing.sequence import pad_sequences

# Step 2: Load the IMDB dataset from TensorFlow
(x_train, y_train), (x_test, y_test) = imdb.load_data(num_words=10000)  # Limit to top 10,000 most frequent words

# Step 3: Preprocess the data
maxlen = 500  # Limit the review length to 500 words for uniformity
x_train = pad_sequences(x_train, maxlen=maxlen)
x_test = pad_sequences(x_test, maxlen=maxlen)

# Step 4: Build the Deep Neural Network model
model = Sequential()
model.add(Embedding(input_dim=10000, output_dim=128, input_length=maxlen))  # Embedding layer for word representation
model.add(GlobalAveragePooling1D())  # Pooling layer to reduce sequence dimension
model.add(Dense(128, activation='relu'))  # 1st hidden layer
model.add(Dense(1, activation='sigmoid'))  # Output layer (sigmoid for binary classification)

# Step 5: Compile the model
model.compile(optimizer='adam', loss='binary_crossentropy', metrics=['accuracy'])

# Step 6: Train the model
model.fit(x_train, y_train, epochs=5, batch_size=64, validation_split=0.1, verbose=1)

# Step 7: Evaluate the model
loss, accuracy = model.evaluate(x_test, y_test)
print(f"\nTest Accuracy: {accuracy:.2f}")

# Step 8: Predict and show comparison for the first 5 reviews
predictions = model.predict(x_test[:5])
print("\nPredicted vs Actual Sentiments:")
for i in range(5):
    print(f"Predicted: {'Positive' if predictions[i][0] >= 0.5 else 'Negative'}, Actual: {'Positive' if y_test[i] == 1 else 'Negative'}")
