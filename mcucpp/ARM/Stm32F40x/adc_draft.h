
typedef void (* AdcCallback)(uint16_t *data, size_t count);

class AdcInterface
{
public:
	static const unsigned MaxSequence = 16;
	
	void Init(Divider divider, ClockSource clock, Reference reference);
	void Disable();
	
	// immediate read
	bool StartImmediate(uint8_t *channel, uint16_t *data, uint8_t count);
	bool ReadImmediate(uint8_t *channel, uint16_t *data, uint8_t count);
	bool ImmediateReady();
	bool ReadImmediate(uint16_t *data);
	
	// continiuos mode
	enum TriggerMode{RisingEdge, FallingEdge, BothEdges};
	enum Trigger{Once, FreeRun, ...};
	void SetTrigger(Trigger trigger, TriggerMode mode);
	void SetSampleTime(uint8_t channel, unsigned adcCycles);
	void SetSequenceCallback(AdcCallback callback);
	bool StartSequence(uint8_t *channels, uint8_t channelsCount, uint16_t *dataBuffer, uint16_t scanCount);
	bool DataReady();
	void Stop();
	
	//
	
	//info
	int ResolutionBits();
	unsigned ToVolts(uint16_t value); // in 10E-4 Volt units 
};