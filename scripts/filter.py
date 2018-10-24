import numpy as np
import scipy.signal as signal
import matplotlib.pyplot as plt

f1, f2 = 50000.0, 80000.0
f=2000000.0
b=signal.firwin(100, [f1, f2], pass_zero=False, fs=f)
b2=signal.firwin(100, [f1*2, f2*2], pass_zero=False, fs=f)

print(b)

w, h = signal.freqz(b)
w2, h2 = signal.freqz(b2)

plt.title('Digital filter frequency response')
plt.plot(w, np.abs(h))
plt.plot(w2, np.abs(h2))

plt.title('Digital filter frequency response')
plt.ylabel('Amplitude Response')
plt.xlabel('Frequency (rad/sample)')
plt.grid()
plt.show()
