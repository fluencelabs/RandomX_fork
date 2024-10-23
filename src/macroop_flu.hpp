	namespace ExecutionPort {
		using type = int;
		constexpr type Null = 0;
		constexpr type P0 = 1;
		constexpr type P1 = 2;
		constexpr type P5 = 4;
		constexpr type P01 = P0 | P1;
		constexpr type P05 = P0 | P5;
		constexpr type P015 = P0 | P1 | P5;
	}
    
	namespace randomx 
	{
//Macro-operation as output of the x86 decoder
	//Usually one macro-op max= one x86 instruction, but 2 instructions are sometimes fused into 1 macro-op
	//Macro-op can consist of 1 or 2 uOPs.
	class MacroOp {
	public:
		MacroOp(const char* name, int size)
			: name_(name), size_(size), latency_(0), uop1_(ExecutionPort::Null), uop2_(ExecutionPort::Null) {}
		MacroOp(const char* name, int size, int latency, ExecutionPort::type uop)
			: name_(name), size_(size), latency_(latency), uop1_(uop), uop2_(ExecutionPort::Null) {}
		MacroOp(const char* name, int size, int latency, ExecutionPort::type uop1, ExecutionPort::type uop2)
			: name_(name), size_(size), latency_(latency), uop1_(uop1), uop2_(uop2) {}
		MacroOp(const MacroOp& parent, bool dependent)
			: name_(parent.name_), size_(parent.size_), latency_(parent.latency_), uop1_(parent.uop1_), uop2_(parent.uop2_), dependent_(dependent) {}
		const char* getName() const {
			return name_;
		}
		int getSize() const {
			return size_;
		}
		int getLatency() const {
			return latency_;
		}
		ExecutionPort::type getUop1() const {
			return uop1_;
		}
		ExecutionPort::type getUop2() const {
			return uop2_;
		}
		bool isSimple() const {
			return uop2_ == ExecutionPort::Null;
		}
		bool isEliminated() const {
			return uop1_ == ExecutionPort::Null;
		}
		bool isDependent() const {
			return dependent_;
		}
		static const MacroOp Add_rr;
		static const MacroOp Add_ri;
		static const MacroOp Lea_sib;
		static const MacroOp Sub_rr;
		static const MacroOp Imul_rr;
		static const MacroOp Imul_r;
		static const MacroOp Mul_r;
		static const MacroOp Mov_rr;
		static const MacroOp Mov_ri64;
		static const MacroOp Xor_rr;
		static const MacroOp Xor_ri;
		static const MacroOp Ror_rcl;
		static const MacroOp Ror_ri;
		static const MacroOp TestJz_fused;
		static const MacroOp Xor_self;
		static const MacroOp Cmp_ri;
		static const MacroOp Setcc_r;
	private:
		const char* name_;
		int size_;
		int latency_;
		ExecutionPort::type uop1_;
		ExecutionPort::type uop2_;
		bool dependent_ = false;
	};
	}
    