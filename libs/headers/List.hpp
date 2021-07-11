namespace CPPExtensions {
	class ChainHead;
	class ChainLink {
		protected:
			static constexpr ChainHead* NullHead = nullptr;
			static constexpr ChainLink* NullLink = nullptr;
			union Prev {
				ChainHead* head;
				ChainLink* back;
			};
		private:
			bool head;
			Prev back;
			ChainLink* next;
			const Utils::size buffer;
	};
}
