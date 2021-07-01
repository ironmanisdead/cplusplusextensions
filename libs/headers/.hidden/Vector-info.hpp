static constexpr bool is_copy = Utils::is_constructible<T, const T&>;
static constexpr bool is_pheonix = Utils::is_constructible<T, T&&>;
static constexpr bool is_assign = Utils::is_assignable<T&, const T&>;
static constexpr bool is_possess = Utils::is_assignable<T&, T&&>;
static constexpr void (*copy)(void*, const void*) = [] () {
	if constexpr (is_copy)
		return &Utils::wrap_construct<T, const T&>;
	else
		return nullptr;
} ();
static constexpr void (*pheonix)(void*, void*) = [] () {
	if constexpr (is_pheonix)
		return &Utils::wrap_construct<T, T&&>;
	else
		return nullptr;
} ();
static constexpr void (*assign)(void*, const void*) = [] () {
	if constexpr (is_assign)
		return &Utils::wrap_assign<T, const T&>;
	else
		return nullptr;
} ();
static constexpr void (*possess)(void*, void*) = [] () {
	if constexpr (is_possess)
		return &Utils::wrap_assign<T, T&&>;
	else
		return nullptr;
} ();
static constexpr TypeData _typeuse =
	{ sizeof(T), Utils::wrap_destruct<T, true>, copy, pheonix, assign, possess };
static constexpr Array _type = GString::typestr<T>;
static constexpr Array _vectype = GString::raycat("Vector<", _type.data, ">");
static constexpr auto _coperr_imp = [] () {
	if constexpr (is_copy)
		return nullptr;
	else
		return GString::raycat(_vectype.data, " has no copy constructor");
} ();
static constexpr const char* _coperr = [] () {
	if constexpr (is_copy)
		return nullptr;
	else
		return _coperr_imp.data;
} ();
static constexpr Array _converr = GString::raycat(_vectype.data, " cannot be converted from ");
static constexpr Array _assignerr = GString::raycat(_vectype.data, " cannot be assigned from ");
static constexpr TypeText _typetext =
	{ _type.data, _vectype.data, _coperr, _converr.data, _assignerr.data };
static constexpr FullType _typeinfo = { &_typeuse, &_typetext };
