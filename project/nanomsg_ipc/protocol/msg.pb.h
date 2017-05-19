// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: msg.proto

#ifndef PROTOBUF_msg_2eproto__INCLUDED
#define PROTOBUF_msg_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3001000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3001000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace msg {

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_msg_2eproto();
void protobuf_InitDefaults_msg_2eproto();
void protobuf_AssignDesc_msg_2eproto();
void protobuf_ShutdownFile_msg_2eproto();

class net_msg;

// ===================================================================

class net_msg : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:msg.net_msg) */ {
 public:
  net_msg();
  virtual ~net_msg();

  net_msg(const net_msg& from);

  inline net_msg& operator=(const net_msg& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const net_msg& default_instance();

  static const net_msg* internal_default_instance();

  void Swap(net_msg* other);

  // implements Message ----------------------------------------------

  inline net_msg* New() const { return New(NULL); }

  net_msg* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const net_msg& from);
  void MergeFrom(const net_msg& from);
  void Clear();
  bool IsInitialized() const;

  size_t ByteSizeLong() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(net_msg* other);
  void UnsafeMergeFrom(const net_msg& from);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required int32 v = 1;
  bool has_v() const;
  void clear_v();
  static const int kVFieldNumber = 1;
  ::google::protobuf::int32 v() const;
  void set_v(::google::protobuf::int32 value);

  // required int32 m_id = 2;
  bool has_m_id() const;
  void clear_m_id();
  static const int kMIdFieldNumber = 2;
  ::google::protobuf::int32 m_id() const;
  void set_m_id(::google::protobuf::int32 value);

  // required int32 s_id = 3;
  bool has_s_id() const;
  void clear_s_id();
  static const int kSIdFieldNumber = 3;
  ::google::protobuf::int32 s_id() const;
  void set_s_id(::google::protobuf::int32 value);

  // required int32 opt = 4;
  bool has_opt() const;
  void clear_opt();
  static const int kOptFieldNumber = 4;
  ::google::protobuf::int32 opt() const;
  void set_opt(::google::protobuf::int32 value);

  // optional string data = 5;
  bool has_data() const;
  void clear_data();
  static const int kDataFieldNumber = 5;
  const ::std::string& data() const;
  void set_data(const ::std::string& value);
  void set_data(const char* value);
  void set_data(const char* value, size_t size);
  ::std::string* mutable_data();
  ::std::string* release_data();
  void set_allocated_data(::std::string* data);

  // @@protoc_insertion_point(class_scope:msg.net_msg)
 private:
  inline void set_has_v();
  inline void clear_has_v();
  inline void set_has_m_id();
  inline void clear_has_m_id();
  inline void set_has_s_id();
  inline void clear_has_s_id();
  inline void set_has_opt();
  inline void clear_has_opt();
  inline void set_has_data();
  inline void clear_has_data();

  // helper for ByteSizeLong()
  size_t RequiredFieldsByteSizeFallback() const;

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable int _cached_size_;
  ::google::protobuf::internal::ArenaStringPtr data_;
  ::google::protobuf::int32 v_;
  ::google::protobuf::int32 m_id_;
  ::google::protobuf::int32 s_id_;
  ::google::protobuf::int32 opt_;
  friend void  protobuf_InitDefaults_msg_2eproto_impl();
  friend void  protobuf_AddDesc_msg_2eproto_impl();
  friend void protobuf_AssignDesc_msg_2eproto();
  friend void protobuf_ShutdownFile_msg_2eproto();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<net_msg> net_msg_default_instance_;

// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// net_msg

// required int32 v = 1;
inline bool net_msg::has_v() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void net_msg::set_has_v() {
  _has_bits_[0] |= 0x00000001u;
}
inline void net_msg::clear_has_v() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void net_msg::clear_v() {
  v_ = 0;
  clear_has_v();
}
inline ::google::protobuf::int32 net_msg::v() const {
  // @@protoc_insertion_point(field_get:msg.net_msg.v)
  return v_;
}
inline void net_msg::set_v(::google::protobuf::int32 value) {
  set_has_v();
  v_ = value;
  // @@protoc_insertion_point(field_set:msg.net_msg.v)
}

// required int32 m_id = 2;
inline bool net_msg::has_m_id() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void net_msg::set_has_m_id() {
  _has_bits_[0] |= 0x00000002u;
}
inline void net_msg::clear_has_m_id() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void net_msg::clear_m_id() {
  m_id_ = 0;
  clear_has_m_id();
}
inline ::google::protobuf::int32 net_msg::m_id() const {
  // @@protoc_insertion_point(field_get:msg.net_msg.m_id)
  return m_id_;
}
inline void net_msg::set_m_id(::google::protobuf::int32 value) {
  set_has_m_id();
  m_id_ = value;
  // @@protoc_insertion_point(field_set:msg.net_msg.m_id)
}

// required int32 s_id = 3;
inline bool net_msg::has_s_id() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void net_msg::set_has_s_id() {
  _has_bits_[0] |= 0x00000004u;
}
inline void net_msg::clear_has_s_id() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void net_msg::clear_s_id() {
  s_id_ = 0;
  clear_has_s_id();
}
inline ::google::protobuf::int32 net_msg::s_id() const {
  // @@protoc_insertion_point(field_get:msg.net_msg.s_id)
  return s_id_;
}
inline void net_msg::set_s_id(::google::protobuf::int32 value) {
  set_has_s_id();
  s_id_ = value;
  // @@protoc_insertion_point(field_set:msg.net_msg.s_id)
}

// required int32 opt = 4;
inline bool net_msg::has_opt() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void net_msg::set_has_opt() {
  _has_bits_[0] |= 0x00000008u;
}
inline void net_msg::clear_has_opt() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void net_msg::clear_opt() {
  opt_ = 0;
  clear_has_opt();
}
inline ::google::protobuf::int32 net_msg::opt() const {
  // @@protoc_insertion_point(field_get:msg.net_msg.opt)
  return opt_;
}
inline void net_msg::set_opt(::google::protobuf::int32 value) {
  set_has_opt();
  opt_ = value;
  // @@protoc_insertion_point(field_set:msg.net_msg.opt)
}

// optional string data = 5;
inline bool net_msg::has_data() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void net_msg::set_has_data() {
  _has_bits_[0] |= 0x00000010u;
}
inline void net_msg::clear_has_data() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void net_msg::clear_data() {
  data_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_data();
}
inline const ::std::string& net_msg::data() const {
  // @@protoc_insertion_point(field_get:msg.net_msg.data)
  return data_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void net_msg::set_data(const ::std::string& value) {
  set_has_data();
  data_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:msg.net_msg.data)
}
inline void net_msg::set_data(const char* value) {
  set_has_data();
  data_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:msg.net_msg.data)
}
inline void net_msg::set_data(const char* value, size_t size) {
  set_has_data();
  data_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:msg.net_msg.data)
}
inline ::std::string* net_msg::mutable_data() {
  set_has_data();
  // @@protoc_insertion_point(field_mutable:msg.net_msg.data)
  return data_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* net_msg::release_data() {
  // @@protoc_insertion_point(field_release:msg.net_msg.data)
  clear_has_data();
  return data_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void net_msg::set_allocated_data(::std::string* data) {
  if (data != NULL) {
    set_has_data();
  } else {
    clear_has_data();
  }
  data_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), data);
  // @@protoc_insertion_point(field_set_allocated:msg.net_msg.data)
}

inline const net_msg* net_msg::internal_default_instance() {
  return &net_msg_default_instance_.get();
}
#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

}  // namespace msg

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_msg_2eproto__INCLUDED
