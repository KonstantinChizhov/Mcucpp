
typedef void (* AdcCallback)(uint16_t *data, size_t count);

class AdcInterface
{
public:
	static const unsigned MaxSequence = 16;
	
	void Init(Divider divider, ClockSource clock, Reference reference);
	void Disable();
	
	// async read
	void StartAsync(uint8_t channel);
	uint16_t Read();
	uint16_t Read(uint8_t channel);
	
	// continiuos mode
	enum TriggerMode{RisingEdge, FallingEdge, BothEdges};
	enum Trigger{Once, FreeRun, ...};
	void SetTrigger(Trigger trigger, TriggerMode mode);
	void SetSampleTime(uint8_t channel, unsigned adcCycles);
	bool Start(uint8_t *channels, uint8_t channelsCount, uint16_t *dataBuffer, uint16_t scanCount, AdcCallback callback);
	bool DataReady();
	void Stop();
	
	//
	
	//info
	int ResolutionBits();
	unsigned ToVolts(uint16_t value); // in 10E-4 Volt units 
};