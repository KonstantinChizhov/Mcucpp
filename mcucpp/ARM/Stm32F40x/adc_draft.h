
typedef void (* AdcCallback)(uint16_t *data, size_t count);
typedef void (* AdcErrorCallback)(Error error);

class AdcInterface
{
public:
	static const unsigned MaxSequence = 16;
	static const unsigned MaxImmediate = 4;
	
	void Init(Divider divider, ClockSource clock, Reference reference);
	void Disable();
	void SetSampleTime(uint8_t channel, unsigned adcCycles);
	void SetResolution(uint8_t bits);
	void SetDivider(Divider divider);
	void SetReference(Reference reference);
	uint8_t ChannelsCount();
	
	enum TriggerMode{RisingEdge, FallingEdge, BothEdges};
	enum SequenceTrigger{Once, FreeRun, ...};
	enum ImmediateTrigger{Once, FreeRun, ...};
	enum Error{Overflow, TransferError, ...};
	
	// immediate
	void SetImmediateTrigger(Trigger trigger, TriggerMode mode);
	bool StartImmediate(uint8_t *channels, uint16_t *data, uint8_t count);
	bool ReadImmediate(uint8_t *channels, uint16_t *data, uint8_t count);
	bool StartImmediate(uint8_t channel);
	uint16_t ReadImmediate();
	uint16_t ReadImmediate(uint8_t channel);
	bool ImmediateReady();
	void StopImmediate();
	
	// continiuos
	void SetSequenceTrigger(Trigger trigger, TriggerMode mode);
	void SetSequenceCallback(AdcCallback callback);
	bool StartSequence(uint8_t *channels, uint8_t channelsCount, uint16_t *dataBuffer, uint16_t scanCount);
	bool SequenceReady();
	void StopSequence();
	
	//info
	int ResolutionBits();
	unsigned ToVolts(uint16_t value); // in 10E-4 Volt units
	unsigned SampleRate();
};