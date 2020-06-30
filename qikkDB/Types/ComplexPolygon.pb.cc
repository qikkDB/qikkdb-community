// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Types/ComplexPolygon.proto

#include "Types/ComplexPolygon.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
extern PROTOBUF_INTERNAL_EXPORT_Types_2fComplexPolygon_2eproto ::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_GeoPoint_Types_2fComplexPolygon_2eproto;
extern PROTOBUF_INTERNAL_EXPORT_Types_2fComplexPolygon_2eproto ::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<1> scc_info_Polygon_Types_2fComplexPolygon_2eproto;
namespace QikkDB {
namespace Types {
class ComplexPolygonDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<ComplexPolygon> _instance;
} _ComplexPolygon_default_instance_;
class PolygonDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<Polygon> _instance;
} _Polygon_default_instance_;
class GeoPointDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<GeoPoint> _instance;
} _GeoPoint_default_instance_;
}  // namespace Types
}  // namespace QikkDB
static void InitDefaultsscc_info_ComplexPolygon_Types_2fComplexPolygon_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::QikkDB::Types::_ComplexPolygon_default_instance_;
    new (ptr) ::QikkDB::Types::ComplexPolygon();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::QikkDB::Types::ComplexPolygon::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<1> scc_info_ComplexPolygon_Types_2fComplexPolygon_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 1, 0, InitDefaultsscc_info_ComplexPolygon_Types_2fComplexPolygon_2eproto}, {
      &scc_info_Polygon_Types_2fComplexPolygon_2eproto.base,}};

static void InitDefaultsscc_info_GeoPoint_Types_2fComplexPolygon_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::QikkDB::Types::_GeoPoint_default_instance_;
    new (ptr) ::QikkDB::Types::GeoPoint();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::QikkDB::Types::GeoPoint::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_GeoPoint_Types_2fComplexPolygon_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_GeoPoint_Types_2fComplexPolygon_2eproto}, {}};

static void InitDefaultsscc_info_Polygon_Types_2fComplexPolygon_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::QikkDB::Types::_Polygon_default_instance_;
    new (ptr) ::QikkDB::Types::Polygon();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::QikkDB::Types::Polygon::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<1> scc_info_Polygon_Types_2fComplexPolygon_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 1, 0, InitDefaultsscc_info_Polygon_Types_2fComplexPolygon_2eproto}, {
      &scc_info_GeoPoint_Types_2fComplexPolygon_2eproto.base,}};

static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_Types_2fComplexPolygon_2eproto[3];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_Types_2fComplexPolygon_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_Types_2fComplexPolygon_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_Types_2fComplexPolygon_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::QikkDB::Types::ComplexPolygon, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::QikkDB::Types::ComplexPolygon, polygons_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::QikkDB::Types::Polygon, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::QikkDB::Types::Polygon, geopoints_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::QikkDB::Types::GeoPoint, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::QikkDB::Types::GeoPoint, latitude_),
  PROTOBUF_FIELD_OFFSET(::QikkDB::Types::GeoPoint, longitude_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::QikkDB::Types::ComplexPolygon)},
  { 6, -1, sizeof(::QikkDB::Types::Polygon)},
  { 12, -1, sizeof(::QikkDB::Types::GeoPoint)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::QikkDB::Types::_ComplexPolygon_default_instance_),
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::QikkDB::Types::_Polygon_default_instance_),
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::QikkDB::Types::_GeoPoint_default_instance_),
};

const char descriptor_table_protodef_Types_2fComplexPolygon_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\032Types/ComplexPolygon.proto\022\014QikkDB.Typ"
  "es\"9\n\016ComplexPolygon\022\'\n\010polygons\030\001 \003(\0132\025"
  ".QikkDB.Types.Polygon\"4\n\007Polygon\022)\n\tgeoP"
  "oints\030\001 \003(\0132\026.QikkDB.Types.GeoPoint\"/\n\010G"
  "eoPoint\022\020\n\010latitude\030\001 \001(\002\022\021\n\tlongitude\030\002"
  " \001(\002b\006proto3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_Types_2fComplexPolygon_2eproto_deps[1] = {
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_Types_2fComplexPolygon_2eproto_sccs[3] = {
  &scc_info_ComplexPolygon_Types_2fComplexPolygon_2eproto.base,
  &scc_info_GeoPoint_Types_2fComplexPolygon_2eproto.base,
  &scc_info_Polygon_Types_2fComplexPolygon_2eproto.base,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_Types_2fComplexPolygon_2eproto_once;
static bool descriptor_table_Types_2fComplexPolygon_2eproto_initialized = false;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_Types_2fComplexPolygon_2eproto = {
  &descriptor_table_Types_2fComplexPolygon_2eproto_initialized, descriptor_table_protodef_Types_2fComplexPolygon_2eproto, "Types/ComplexPolygon.proto", 212,
  &descriptor_table_Types_2fComplexPolygon_2eproto_once, descriptor_table_Types_2fComplexPolygon_2eproto_sccs, descriptor_table_Types_2fComplexPolygon_2eproto_deps, 3, 0,
  schemas, file_default_instances, TableStruct_Types_2fComplexPolygon_2eproto::offsets,
  file_level_metadata_Types_2fComplexPolygon_2eproto, 3, file_level_enum_descriptors_Types_2fComplexPolygon_2eproto, file_level_service_descriptors_Types_2fComplexPolygon_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_Types_2fComplexPolygon_2eproto = (  ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_Types_2fComplexPolygon_2eproto), true);
namespace QikkDB {
namespace Types {

// ===================================================================

void ComplexPolygon::InitAsDefaultInstance() {
}
class ComplexPolygon::_Internal {
 public:
};

ComplexPolygon::ComplexPolygon()
  : ::PROTOBUF_NAMESPACE_ID::Message(), _internal_metadata_(nullptr) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:QikkDB.Types.ComplexPolygon)
}
ComplexPolygon::ComplexPolygon(const ComplexPolygon& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _internal_metadata_(nullptr),
      polygons_(from.polygons_) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  // @@protoc_insertion_point(copy_constructor:QikkDB.Types.ComplexPolygon)
}

void ComplexPolygon::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_ComplexPolygon_Types_2fComplexPolygon_2eproto.base);
}

ComplexPolygon::~ComplexPolygon() {
  // @@protoc_insertion_point(destructor:QikkDB.Types.ComplexPolygon)
  SharedDtor();
}

void ComplexPolygon::SharedDtor() {
}

void ComplexPolygon::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const ComplexPolygon& ComplexPolygon::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_ComplexPolygon_Types_2fComplexPolygon_2eproto.base);
  return *internal_default_instance();
}


void ComplexPolygon::Clear() {
// @@protoc_insertion_point(message_clear_start:QikkDB.Types.ComplexPolygon)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  polygons_.Clear();
  _internal_metadata_.Clear();
}

const char* ComplexPolygon::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // repeated .QikkDB.Types.Polygon polygons = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 10)) {
          ptr -= 1;
          do {
            ptr += 1;
            ptr = ctx->ParseMessage(_internal_add_polygons(), ptr);
            CHK_(ptr);
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::ExpectTag<10>(ptr));
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag, &_internal_metadata_, ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* ComplexPolygon::InternalSerializeWithCachedSizesToArray(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:QikkDB.Types.ComplexPolygon)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // repeated .QikkDB.Types.Polygon polygons = 1;
  for (unsigned int i = 0,
      n = static_cast<unsigned int>(this->_internal_polygons_size()); i < n; i++) {
    stream->EnsureSpace(&target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessageToArray(1, this->_internal_polygons(i), target, stream);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:QikkDB.Types.ComplexPolygon)
  return target;
}

size_t ComplexPolygon::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:QikkDB.Types.ComplexPolygon)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated .QikkDB.Types.Polygon polygons = 1;
  total_size += 1UL * this->_internal_polygons_size();
  for (const auto& msg : this->polygons_) {
    total_size +=
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(msg);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void ComplexPolygon::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:QikkDB.Types.ComplexPolygon)
  GOOGLE_DCHECK_NE(&from, this);
  const ComplexPolygon* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<ComplexPolygon>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:QikkDB.Types.ComplexPolygon)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:QikkDB.Types.ComplexPolygon)
    MergeFrom(*source);
  }
}

void ComplexPolygon::MergeFrom(const ComplexPolygon& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:QikkDB.Types.ComplexPolygon)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  polygons_.MergeFrom(from.polygons_);
}

void ComplexPolygon::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:QikkDB.Types.ComplexPolygon)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void ComplexPolygon::CopyFrom(const ComplexPolygon& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:QikkDB.Types.ComplexPolygon)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ComplexPolygon::IsInitialized() const {
  return true;
}

void ComplexPolygon::InternalSwap(ComplexPolygon* other) {
  using std::swap;
  _internal_metadata_.Swap(&other->_internal_metadata_);
  polygons_.InternalSwap(&other->polygons_);
}

::PROTOBUF_NAMESPACE_ID::Metadata ComplexPolygon::GetMetadata() const {
  return GetMetadataStatic();
}


// ===================================================================

void Polygon::InitAsDefaultInstance() {
}
class Polygon::_Internal {
 public:
};

Polygon::Polygon()
  : ::PROTOBUF_NAMESPACE_ID::Message(), _internal_metadata_(nullptr) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:QikkDB.Types.Polygon)
}
Polygon::Polygon(const Polygon& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _internal_metadata_(nullptr),
      geopoints_(from.geopoints_) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  // @@protoc_insertion_point(copy_constructor:QikkDB.Types.Polygon)
}

void Polygon::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_Polygon_Types_2fComplexPolygon_2eproto.base);
}

Polygon::~Polygon() {
  // @@protoc_insertion_point(destructor:QikkDB.Types.Polygon)
  SharedDtor();
}

void Polygon::SharedDtor() {
}

void Polygon::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const Polygon& Polygon::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_Polygon_Types_2fComplexPolygon_2eproto.base);
  return *internal_default_instance();
}


void Polygon::Clear() {
// @@protoc_insertion_point(message_clear_start:QikkDB.Types.Polygon)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  geopoints_.Clear();
  _internal_metadata_.Clear();
}

const char* Polygon::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // repeated .QikkDB.Types.GeoPoint geoPoints = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 10)) {
          ptr -= 1;
          do {
            ptr += 1;
            ptr = ctx->ParseMessage(_internal_add_geopoints(), ptr);
            CHK_(ptr);
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::ExpectTag<10>(ptr));
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag, &_internal_metadata_, ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* Polygon::InternalSerializeWithCachedSizesToArray(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:QikkDB.Types.Polygon)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // repeated .QikkDB.Types.GeoPoint geoPoints = 1;
  for (unsigned int i = 0,
      n = static_cast<unsigned int>(this->_internal_geopoints_size()); i < n; i++) {
    stream->EnsureSpace(&target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessageToArray(1, this->_internal_geopoints(i), target, stream);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:QikkDB.Types.Polygon)
  return target;
}

size_t Polygon::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:QikkDB.Types.Polygon)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated .QikkDB.Types.GeoPoint geoPoints = 1;
  total_size += 1UL * this->_internal_geopoints_size();
  for (const auto& msg : this->geopoints_) {
    total_size +=
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(msg);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void Polygon::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:QikkDB.Types.Polygon)
  GOOGLE_DCHECK_NE(&from, this);
  const Polygon* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<Polygon>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:QikkDB.Types.Polygon)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:QikkDB.Types.Polygon)
    MergeFrom(*source);
  }
}

void Polygon::MergeFrom(const Polygon& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:QikkDB.Types.Polygon)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  geopoints_.MergeFrom(from.geopoints_);
}

void Polygon::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:QikkDB.Types.Polygon)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Polygon::CopyFrom(const Polygon& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:QikkDB.Types.Polygon)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Polygon::IsInitialized() const {
  return true;
}

void Polygon::InternalSwap(Polygon* other) {
  using std::swap;
  _internal_metadata_.Swap(&other->_internal_metadata_);
  geopoints_.InternalSwap(&other->geopoints_);
}

::PROTOBUF_NAMESPACE_ID::Metadata Polygon::GetMetadata() const {
  return GetMetadataStatic();
}


// ===================================================================

void GeoPoint::InitAsDefaultInstance() {
}
class GeoPoint::_Internal {
 public:
};

GeoPoint::GeoPoint()
  : ::PROTOBUF_NAMESPACE_ID::Message(), _internal_metadata_(nullptr) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:QikkDB.Types.GeoPoint)
}
GeoPoint::GeoPoint(const GeoPoint& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _internal_metadata_(nullptr) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::memcpy(&latitude_, &from.latitude_,
    static_cast<size_t>(reinterpret_cast<char*>(&longitude_) -
    reinterpret_cast<char*>(&latitude_)) + sizeof(longitude_));
  // @@protoc_insertion_point(copy_constructor:QikkDB.Types.GeoPoint)
}

void GeoPoint::SharedCtor() {
  ::memset(&latitude_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&longitude_) -
      reinterpret_cast<char*>(&latitude_)) + sizeof(longitude_));
}

GeoPoint::~GeoPoint() {
  // @@protoc_insertion_point(destructor:QikkDB.Types.GeoPoint)
  SharedDtor();
}

void GeoPoint::SharedDtor() {
}

void GeoPoint::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const GeoPoint& GeoPoint::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_GeoPoint_Types_2fComplexPolygon_2eproto.base);
  return *internal_default_instance();
}


void GeoPoint::Clear() {
// @@protoc_insertion_point(message_clear_start:QikkDB.Types.GeoPoint)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  ::memset(&latitude_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&longitude_) -
      reinterpret_cast<char*>(&latitude_)) + sizeof(longitude_));
  _internal_metadata_.Clear();
}

const char* GeoPoint::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // float latitude = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 13)) {
          latitude_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<float>(ptr);
          ptr += sizeof(float);
        } else goto handle_unusual;
        continue;
      // float longitude = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 21)) {
          longitude_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<float>(ptr);
          ptr += sizeof(float);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag, &_internal_metadata_, ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* GeoPoint::InternalSerializeWithCachedSizesToArray(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:QikkDB.Types.GeoPoint)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // float latitude = 1;
  if (!(this->latitude() <= 0 && this->latitude() >= 0)) {
    stream->EnsureSpace(&target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteFloatToArray(1, this->_internal_latitude(), target);
  }

  // float longitude = 2;
  if (!(this->longitude() <= 0 && this->longitude() >= 0)) {
    stream->EnsureSpace(&target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteFloatToArray(2, this->_internal_longitude(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:QikkDB.Types.GeoPoint)
  return target;
}

size_t GeoPoint::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:QikkDB.Types.GeoPoint)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // float latitude = 1;
  if (!(this->latitude() <= 0 && this->latitude() >= 0)) {
    total_size += 1 + 4;
  }

  // float longitude = 2;
  if (!(this->longitude() <= 0 && this->longitude() >= 0)) {
    total_size += 1 + 4;
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void GeoPoint::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:QikkDB.Types.GeoPoint)
  GOOGLE_DCHECK_NE(&from, this);
  const GeoPoint* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<GeoPoint>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:QikkDB.Types.GeoPoint)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:QikkDB.Types.GeoPoint)
    MergeFrom(*source);
  }
}

void GeoPoint::MergeFrom(const GeoPoint& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:QikkDB.Types.GeoPoint)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (!(from.latitude() <= 0 && from.latitude() >= 0)) {
    _internal_set_latitude(from._internal_latitude());
  }
  if (!(from.longitude() <= 0 && from.longitude() >= 0)) {
    _internal_set_longitude(from._internal_longitude());
  }
}

void GeoPoint::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:QikkDB.Types.GeoPoint)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void GeoPoint::CopyFrom(const GeoPoint& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:QikkDB.Types.GeoPoint)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool GeoPoint::IsInitialized() const {
  return true;
}

void GeoPoint::InternalSwap(GeoPoint* other) {
  using std::swap;
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(latitude_, other->latitude_);
  swap(longitude_, other->longitude_);
}

::PROTOBUF_NAMESPACE_ID::Metadata GeoPoint::GetMetadata() const {
  return GetMetadataStatic();
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace Types
}  // namespace QikkDB
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::QikkDB::Types::ComplexPolygon* Arena::CreateMaybeMessage< ::QikkDB::Types::ComplexPolygon >(Arena* arena) {
  return Arena::CreateInternal< ::QikkDB::Types::ComplexPolygon >(arena);
}
template<> PROTOBUF_NOINLINE ::QikkDB::Types::Polygon* Arena::CreateMaybeMessage< ::QikkDB::Types::Polygon >(Arena* arena) {
  return Arena::CreateInternal< ::QikkDB::Types::Polygon >(arena);
}
template<> PROTOBUF_NOINLINE ::QikkDB::Types::GeoPoint* Arena::CreateMaybeMessage< ::QikkDB::Types::GeoPoint >(Arena* arena) {
  return Arena::CreateInternal< ::QikkDB::Types::GeoPoint >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>