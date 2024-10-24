Project Title: Simple Speech Detector System in C to detect the words Yes/No

Methodology:
-First the word “Yes” or “No” is recorded on Cool Edit and saved as a .txt file.
-After that the following algorithm is followed to make the code:
1. Open the file in read mode and extract all its contents.
2. Process the content in batches of 320 samples each.
3. Find the average of all samples to calculate the DC Shift.
4. Normalize the wave by setting the max/min limit as 5000. In the code absolute value of first sample is taken as max value initially then all samples are brought into a ‘for’ loop to find the actual max value. If the max value is found the exceed the limit of 5000 then the normalization or scale factor is calculate by (max_value/limit) else it is taken as 1.0
5. Then from all sample DC Shift is subtracted and normalization factor is multiplied.
6. Calculate energy and ZCR
7. Finally if the ZCR of the last 40% of batches containing 320 samples is high then it is a “Yes” else it is a “No”.
