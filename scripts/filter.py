import numpy as np
import scipy.signal as signal
import matplotlib.pyplot as plt

f1, f2 = 50000.0, 80000.0
f=2000000.0
b=signal.firwin(100, [f1/f, f2/f], pass_zero=False)
b2=signal.firwin(100, [f1*2/f, f2*2/f], pass_zero=False)

b3,a3=signal.butter(3, [f1/f, f2/f], btype='bandpass')
b4,a4=signal.butter(3, [f1*2/f, f2*2/f], btype='bandpass')

print(b3, a3)
print(b4, a4)

w, h = signal.freqz(b)
w2, h2 = signal.freqz(b2)
w3, h3 = signal.freqz(b3,a3)
w4, h4 = signal.freqz(b4,a4)

plt.title('Digital filter frequency response')
plt.plot(w, np.abs(h))
plt.plot(w2, np.abs(h2))
plt.plot(w3, np.abs(h3))
plt.plot(w4, np.abs(h4))

plt.title('Digital filter frequency response')
plt.ylabel('Amplitude Response')
plt.xlabel('Frequency (rad/sample)')
plt.grid()
plt.show()
