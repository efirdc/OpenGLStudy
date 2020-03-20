// This file contains all native_functions that can be registered to
// and the schema string that they should be registered with

Tensor __and__(const Tensor & self, Scalar other); // aten::__and__.Scalar(Tensor self, Scalar other) -> Tensor
Tensor __and__(const Tensor & self, const Tensor & other); // aten::__and__.Tensor(Tensor self, Tensor other) -> Tensor
Tensor & __iand__(Tensor & self, Scalar other); // aten::__iand__.Scalar(Tensor(a!) self, Scalar other) -> Tensor(a!)
Tensor & __iand__(Tensor & self, const Tensor & other); // aten::__iand__.Tensor(Tensor(a!) self, Tensor other) -> Tensor(a!)
Tensor & __ilshift__(Tensor & self, Scalar other); // aten::__ilshift__.Scalar(Tensor(a!) self, Scalar other) -> Tensor(a!)
Tensor & __ilshift__(Tensor & self, const Tensor & other); // aten::__ilshift__.Tensor(Tensor(a!) self, Tensor other) -> Tensor(a!)
Tensor & __ior__(Tensor & self, Scalar other); // aten::__ior__.Scalar(Tensor(a!) self, Scalar other) -> Tensor(a!)
Tensor & __ior__(Tensor & self, const Tensor & other); // aten::__ior__.Tensor(Tensor(a!) self, Tensor other) -> Tensor(a!)
Tensor & __irshift__(Tensor & self, Scalar other); // aten::__irshift__.Scalar(Tensor(a!) self, Scalar other) -> Tensor(a!)
Tensor & __irshift__(Tensor & self, const Tensor & other); // aten::__irshift__.Tensor(Tensor(a!) self, Tensor other) -> Tensor(a!)
Tensor __lshift__(const Tensor & self, Scalar other); // aten::__lshift__.Scalar(Tensor self, Scalar other) -> Tensor
Tensor __lshift__(const Tensor & self, const Tensor & other); // aten::__lshift__.Tensor(Tensor self, Tensor other) -> Tensor
Tensor __or__(const Tensor & self, Scalar other); // aten::__or__.Scalar(Tensor self, Scalar other) -> Tensor
Tensor __or__(const Tensor & self, const Tensor & other); // aten::__or__.Tensor(Tensor self, Tensor other) -> Tensor
Tensor __rshift__(const Tensor & self, Scalar other); // aten::__rshift__.Scalar(Tensor self, Scalar other) -> Tensor
Tensor __rshift__(const Tensor & self, const Tensor & other); // aten::__rshift__.Tensor(Tensor self, Tensor other) -> Tensor
Tensor _adaptive_avg_pool2d(const Tensor & self, IntArrayRef output_size); // aten::_adaptive_avg_pool2d(Tensor self, int[2] output_size) -> Tensor
Tensor _adaptive_avg_pool2d_backward(const Tensor & grad_output, const Tensor & self); // aten::_adaptive_avg_pool2d_backward(Tensor grad_output, Tensor self) -> Tensor
Tensor _addr(const Tensor & self, const Tensor & vec1, const Tensor & vec2, Scalar beta, Scalar alpha); // aten::_addr(Tensor self, Tensor vec1, Tensor vec2, *, Scalar beta=1, Scalar alpha=1) -> Tensor
Tensor & _addr_(Tensor & self, const Tensor & vec1, const Tensor & vec2, Scalar beta, Scalar alpha); // aten::_addr_(Tensor(a!) self, Tensor vec1, Tensor vec2, *, Scalar beta=1, Scalar alpha=1) -> Tensor(a!)
Tensor & _addr_out(Tensor & out, const Tensor & self, const Tensor & vec1, const Tensor & vec2, Scalar beta, Scalar alpha); // aten::_addr.out(Tensor self, Tensor vec1, Tensor vec2, *, Scalar beta=1, Scalar alpha=1, Tensor(a!) out) -> Tensor(a!)
Tensor _cat(TensorList tensors, int64_t dim); // aten::_cat(Tensor[] tensors, int dim=0) -> Tensor
Tensor & _cat_out(Tensor & out, TensorList tensors, int64_t dim); // aten::_cat.out(Tensor[] tensors, int dim=0, *, Tensor(a!) out) -> Tensor(a!)
Tensor _cdist_backward(const Tensor & grad, const Tensor & x1, const Tensor & x2, double p, const Tensor & cdist); // aten::_cdist_backward(Tensor grad, Tensor x1, Tensor x2, float p, Tensor cdist) -> Tensor
Tensor _cholesky_helper(const Tensor & self, bool upper); // aten::_cholesky_helper(Tensor self, bool upper) -> Tensor
Tensor _cholesky_solve_helper(const Tensor & self, const Tensor & A, bool upper); // aten::_cholesky_solve_helper(Tensor self, Tensor A, bool upper) -> Tensor
Tensor & _coalesced_(Tensor & self, bool coalesced); // aten::_coalesced_(Tensor(a!) self, bool coalesced) -> Tensor(a!)
Tensor _copy_from(const Tensor & self, const Tensor & dst, bool non_blocking); // aten::_copy_from(Tensor self, Tensor dst, bool non_blocking=False) -> Tensor
std::tuple<Tensor,Tensor> _ctc_loss(const Tensor & log_probs, const Tensor & targets, IntArrayRef input_lengths, IntArrayRef target_lengths, int64_t blank, bool zero_infinity); // aten::_ctc_loss(Tensor log_probs, Tensor targets, int[] input_lengths, int[] target_lengths, int blank=0, bool zero_infinity=False) -> (Tensor, Tensor)
Tensor _ctc_loss_backward(const Tensor & grad, const Tensor & log_probs, const Tensor & targets, IntArrayRef input_lengths, IntArrayRef target_lengths, const Tensor & neg_log_likelihood, const Tensor & log_alpha, int64_t blank, bool zero_infinity); // aten::_ctc_loss_backward(Tensor grad, Tensor log_probs, Tensor targets, int[] input_lengths, int[] target_lengths, Tensor neg_log_likelihood, Tensor log_alpha, int blank, bool zero_infinity=False) -> Tensor
std::tuple<Tensor,Tensor> _cudnn_ctc_loss(const Tensor & log_probs, const Tensor & targets, IntArrayRef input_lengths, IntArrayRef target_lengths, int64_t blank, bool deterministic, bool zero_infinity); // aten::_cudnn_ctc_loss(Tensor log_probs, Tensor targets, int[] input_lengths, int[] target_lengths, int blank, bool deterministic, bool zero_infinity) -> (Tensor, Tensor)
Tensor _cudnn_init_dropout_state(double dropout, bool train, int64_t dropout_seed, const TensorOptions & options); // aten::_cudnn_init_dropout_state(float dropout, bool train, int dropout_seed, *, ScalarType dtype, Layout layout, Device device, bool pin_memory=False) -> Tensor
std::tuple<Tensor,Tensor,Tensor,Tensor,Tensor> _cudnn_rnn(const Tensor & input, TensorList weight, int64_t weight_stride0, const Tensor & weight_buf, const Tensor & hx, const Tensor & cx, int64_t mode, int64_t hidden_size, int64_t num_layers, bool batch_first, double dropout, bool train, bool bidirectional, IntArrayRef batch_sizes, const Tensor & dropout_state); // aten::_cudnn_rnn(Tensor input, Tensor[] weight, int weight_stride0, Tensor? weight_buf, Tensor hx, Tensor? cx, int mode, int hidden_size, int num_layers, bool batch_first, float dropout, bool train, bool bidirectional, int[] batch_sizes, Tensor? dropout_state) -> (Tensor, Tensor, Tensor, Tensor, Tensor)
std::tuple<Tensor,Tensor,Tensor,std::vector<Tensor>> _cudnn_rnn_backward(const Tensor & input, TensorList weight, int64_t weight_stride0, const Tensor & weight_buf, const Tensor & hx, const Tensor & cx, const Tensor & output, const Tensor & grad_output, const Tensor & grad_hy, const Tensor & grad_cy, int64_t mode, int64_t hidden_size, int64_t num_layers, bool batch_first, double dropout, bool train, bool bidirectional, IntArrayRef batch_sizes, const Tensor & dropout_state, const Tensor & reserve, std::array<bool,4> output_mask); // aten::_cudnn_rnn_backward(Tensor input, Tensor[] weight, int weight_stride0, Tensor weight_buf, Tensor hx, Tensor? cx, Tensor output, Tensor? grad_output, Tensor? grad_hy, Tensor? grad_cy, int mode, int hidden_size, int num_layers, bool batch_first, float dropout, bool train, bool bidirectional, int[] batch_sizes, Tensor? dropout_state, Tensor reserve, bool[4] output_mask) -> (Tensor, Tensor, Tensor, Tensor[])
Tensor _cudnn_rnn_flatten_weight(TensorList weight_arr, int64_t weight_stride0, int64_t input_size, int64_t mode, int64_t hidden_size, int64_t num_layers, bool batch_first, bool bidirectional); // aten::_cudnn_rnn_flatten_weight(Tensor[] weight_arr, int weight_stride0, int input_size, int mode, int hidden_size, int num_layers, bool batch_first, bool bidirectional) -> Tensor
Tensor _cumprod(const Tensor & self, int64_t dim); // aten::_cumprod(Tensor self, int dim) -> Tensor
Tensor & _cumprod_out(Tensor & out, const Tensor & self, int64_t dim); // aten::_cumprod.out(Tensor self, int dim, *, Tensor(a!) out) -> Tensor(a!)
Tensor _cumsum(const Tensor & self, int64_t dim); // aten::_cumsum(Tensor self, int dim) -> Tensor
Tensor & _cumsum_out(Tensor & out, const Tensor & self, int64_t dim); // aten::_cumsum.out(Tensor self, int dim, *, Tensor(a!) out) -> Tensor(a!)
int64_t _dimI(const Tensor & self); // aten::_dimI(Tensor self) -> int
int64_t _dimV(const Tensor & self); // aten::_dimV(Tensor self) -> int
Tensor _dirichlet_grad(const Tensor & x, const Tensor & alpha, const Tensor & total); // aten::_dirichlet_grad(Tensor x, Tensor alpha, Tensor total) -> Tensor
std::tuple<Tensor,Tensor,Tensor,Tensor> _embedding_bag(const Tensor & weight, const Tensor & indices, const Tensor & offsets, bool scale_grad_by_freq, int64_t mode, bool sparse, const Tensor & per_sample_weights); // aten::_embedding_bag(Tensor weight, Tensor indices, Tensor offsets, bool scale_grad_by_freq=False, int mode=0, bool sparse=False, Tensor? per_sample_weights=None) -> (Tensor, Tensor, Tensor, Tensor)
Tensor _embedding_bag_dense_backward(const Tensor & grad, const Tensor & indices, const Tensor & offsets, const Tensor & offset2bag, const Tensor & bag_size, const Tensor & maximum_indices, int64_t num_weights, bool scale_grad_by_freq, int64_t mode, const Tensor & per_sample_weights); // aten::_embedding_bag_dense_backward(Tensor grad, Tensor indices, Tensor offsets, Tensor offset2bag, Tensor bag_size, Tensor maximum_indices, int num_weights, bool scale_grad_by_freq, int mode, Tensor? per_sample_weights) -> Tensor
Tensor _embedding_bag_per_sample_weights_backward(const Tensor & grad, const Tensor & weight, const Tensor & indices, const Tensor & offsets, const Tensor & offset2bag, int64_t mode); // aten::_embedding_bag_per_sample_weights_backward(Tensor grad, Tensor weight, Tensor indices, Tensor offsets, Tensor offset2bag, int mode) -> Tensor
Tensor _empty_affine_quantized(IntArrayRef size, const TensorOptions & options, double scale, int64_t zero_point, c10::optional<MemoryFormat> memory_format); // aten::_empty_affine_quantized(int[] size, *, ScalarType? dtype=None, Layout? layout=None, Device? device=None, bool? pin_memory=None, float scale=1, int zero_point=0, MemoryFormat? memory_format=contiguous_format) -> Tensor
Tensor _empty_per_channel_affine_quantized(IntArrayRef size, const Tensor & scales, const Tensor & zero_points, int64_t axis, const TensorOptions & options, c10::optional<MemoryFormat> memory_format); // aten::_empty_per_channel_affine_quantized(int[] size, *, Tensor scales, Tensor zero_points, int axis, ScalarType? dtype=None, Layout? layout=None, Device? device=None, bool? pin_memory=None, MemoryFormat? memory_format=contiguous_format) -> Tensor
Tensor _fft_with_size(const Tensor & self, int64_t signal_ndim, bool complex_input, bool complex_output, bool inverse, IntArrayRef checked_signal_sizes, bool normalized, bool onesided, IntArrayRef output_sizes); // aten::_fft_with_size(Tensor self, int signal_ndim, bool complex_input, bool complex_output, bool inverse, int[] checked_signal_sizes, bool normalized, bool onesided, int[] output_sizes) -> Tensor
std::tuple<Tensor,Tensor> _fused_dropout(const Tensor & self, double p, Generator * generator); // aten::_fused_dropout(Tensor self, float p, Generator? generator=None) -> (Tensor, Tensor)
Tensor & _index_copy_(Tensor & self, int64_t dim, const Tensor & index, const Tensor & source); // aten::_index_copy_(Tensor(a!) self, int dim, Tensor index, Tensor source) -> Tensor(a!)
Tensor & _index_put_impl_(Tensor & self, TensorList indices, const Tensor & values, bool accumulate, bool unsafe); // aten::_index_put_impl_(Tensor(a!) self, Tensor?[] indices, Tensor values, bool accumulate=False, bool unsafe=False) -> Tensor(a!)
Tensor _indices(const Tensor & self); // aten::_indices(Tensor(a) self) -> Tensor(a)
Tensor _inverse_helper(const Tensor & self); // aten::_inverse_helper(Tensor self) -> Tensor
Scalar _local_scalar_dense(const Tensor & self); // aten::_local_scalar_dense(Tensor self) -> Scalar
Tensor _log_softmax(const Tensor & self, int64_t dim, bool half_to_float); // aten::_log_softmax(Tensor self, int dim, bool half_to_float) -> Tensor
Tensor _log_softmax_backward_data(const Tensor & grad_output, const Tensor & output, int64_t dim, const Tensor & self); // aten::_log_softmax_backward_data(Tensor grad_output, Tensor output, int dim, Tensor self) -> Tensor
Tensor _lu_solve_helper(const Tensor & self, const Tensor & LU_data, const Tensor & LU_pivots); // aten::_lu_solve_helper(Tensor self, Tensor LU_data, Tensor LU_pivots) -> Tensor
std::tuple<Tensor,Tensor,Tensor> _lu_with_info(const Tensor & self, bool pivot, bool check_errors); // aten::_lu_with_info(Tensor self, bool pivot=True, bool check_errors=True) -> (Tensor, Tensor, Tensor)
Tensor _make_per_channel_quantized_tensor(const Tensor & self, const Tensor & scale, const Tensor & zero_point, int64_t axis); // aten::_make_per_channel_quantized_tensor(Tensor self, Tensor scale, Tensor zero_point, int axis) -> Tensor
Tensor _make_per_tensor_quantized_tensor(const Tensor & self, double scale, int64_t zero_point); // aten::_make_per_tensor_quantized_tensor(Tensor self, float scale, int zero_point) -> Tensor
Tensor _masked_scale(const Tensor & self, const Tensor & mask, double scale); // aten::_masked_scale(Tensor self, Tensor mask, float scale) -> Tensor
std::tuple<Tensor,Tensor> _max(const Tensor & self, int64_t dim, bool keepdim); // aten::_max(Tensor self, int dim, bool keepdim=False) -> (Tensor, Tensor)
std::tuple<Tensor &,Tensor &> _max_out(Tensor & max, Tensor & max_indices, const Tensor & self, int64_t dim, bool keepdim); // aten::_max.max(Tensor self, int dim, bool keepdim=False, *, Tensor(a!) max, Tensor(b!) max_indices) -> (Tensor(a!), Tensor(b!))
std::tuple<Tensor,Tensor> _min(const Tensor & self, int64_t dim, bool keepdim); // aten::_min(Tensor self, int dim, bool keepdim=False) -> (Tensor, Tensor)
std::tuple<Tensor &,Tensor &> _min_out(Tensor & min, Tensor & min_indices, const Tensor & self, int64_t dim, bool keepdim); // aten::_min.min(Tensor self, int dim, bool keepdim=False, *, Tensor(a!) min, Tensor(b!) min_indices) -> (Tensor(a!), Tensor(b!))
Tensor _mkldnn_reshape(const Tensor & self, IntArrayRef shape); // aten::_mkldnn_reshape(Tensor self, int[] shape) -> Tensor
Tensor _mkldnn_transpose(const Tensor & self, int64_t dim0, int64_t dim1); // aten::_mkldnn_transpose(Tensor self, int dim0, int dim1) -> Tensor
Tensor & _mkldnn_transpose_(Tensor & self, int64_t dim0, int64_t dim1); // aten::_mkldnn_transpose_(Tensor(a!) self, int dim0, int dim1) -> Tensor(a!)
std::tuple<Tensor,Tensor> _mode(const Tensor & self, int64_t dim, bool keepdim); // aten::_mode(Tensor self, int dim=-1, bool keepdim=False) -> (Tensor, Tensor)
std::tuple<Tensor &,Tensor &> _mode_out(Tensor & values, Tensor & indices, const Tensor & self, int64_t dim, bool keepdim); // aten::_mode.values(Tensor self, int dim=-1, bool keepdim=False, *, Tensor(a!) values, Tensor(b!) indices) -> (Tensor(a!), Tensor(b!))
Tensor _multinomial_alias_draw(const Tensor & J, const Tensor & q, int64_t num_samples, Generator * generator); // aten::_multinomial_alias_draw(Tensor J, Tensor q, int num_samples, *, Generator? generator=None) -> Tensor
std::tuple<Tensor,Tensor> _multinomial_alias_setup(const Tensor & probs); // aten::_multinomial_alias_setup(Tensor probs) -> (Tensor, Tensor)
Tensor _nnpack_spatial_convolution(const Tensor & input, const Tensor & weight, const Tensor & bias, IntArrayRef padding, IntArrayRef stride); // aten::_nnpack_spatial_convolution(Tensor input, Tensor weight, Tensor? bias, int[2] padding, int[2] stride=1) -> Tensor
int64_t _nnz(const Tensor & self); // aten::_nnz(Tensor self) -> int
std::tuple<Tensor,Tensor> _pack_padded_sequence(const Tensor & input, const Tensor & lengths, bool batch_first); // aten::_pack_padded_sequence(Tensor input, Tensor lengths, bool batch_first) -> (Tensor, Tensor)
Tensor _pdist_backward(const Tensor & grad, const Tensor & self, double p, const Tensor & pdist); // aten::_pdist_backward(Tensor grad, Tensor self, float p, Tensor pdist) -> Tensor
Tensor _pdist_forward(const Tensor & self, double p); // aten::_pdist_forward(Tensor self, float p=2) -> Tensor
std::tuple<Tensor,Tensor> _qr_helper(const Tensor & self, bool some); // aten::_qr_helper(Tensor self, bool some) -> (Tensor, Tensor)
Tensor _s_where(const Tensor & condition, const Tensor & self, const Tensor & other); // aten::_s_where(Tensor condition, Tensor self, Tensor other) -> Tensor
Tensor _sample_dirichlet(const Tensor & self, Generator * generator); // aten::_sample_dirichlet(Tensor self, Generator? generator=None) -> Tensor
Tensor _softmax(const Tensor & self, int64_t dim, bool half_to_float); // aten::_softmax(Tensor self, int dim, bool half_to_float) -> Tensor
Tensor _softmax_backward_data(const Tensor & grad_output, const Tensor & output, int64_t dim, const Tensor & self); // aten::_softmax_backward_data(Tensor grad_output, Tensor output, int dim, Tensor self) -> Tensor
std::tuple<Tensor,Tensor> _solve_helper(const Tensor & self, const Tensor & A); // aten::_solve_helper(Tensor self, Tensor A) -> (Tensor, Tensor)
Tensor _sparse_addmm(const Tensor & self, const Tensor & sparse, const Tensor & dense, Scalar beta, Scalar alpha); // aten::_sparse_addmm(Tensor self, Tensor sparse, Tensor dense, *, Scalar beta=1, Scalar alpha=1) -> Tensor
Tensor _sparse_coo_tensor_with_dims(int64_t sparse_dim, int64_t dense_dim, IntArrayRef size, const TensorOptions & options); // aten::_sparse_coo_tensor_with_dims(int sparse_dim, int dense_dim, int[] size, *, ScalarType dtype, Layout layout, Device device, bool pin_memory=False) -> Tensor
Tensor _sparse_coo_tensor_with_dims_and_tensors(int64_t sparse_dim, int64_t dense_dim, IntArrayRef size, const Tensor & indices, const Tensor & values, const TensorOptions & options); // aten::_sparse_coo_tensor_with_dims_and_tensors(int sparse_dim, int dense_dim, int[] size, Tensor indices, Tensor values, *, ScalarType dtype, Layout layout, Device device, bool pin_memory=False) -> Tensor
Tensor _sparse_sum(const Tensor & self, IntArrayRef dim); // aten::_sparse_sum.dim(Tensor self, int[1] dim) -> Tensor
Tensor _sparse_sum_backward(const Tensor & grad, const Tensor & self, IntArrayRef dim); // aten::_sparse_sum_backward(Tensor grad, Tensor self, int[] dim) -> Tensor
Tensor _standard_gamma(const Tensor & self, Generator * generator); // aten::_standard_gamma(Tensor self, Generator? generator=None) -> Tensor
Tensor _standard_gamma_grad(const Tensor & self, const Tensor & output); // aten::_standard_gamma_grad(Tensor self, Tensor output) -> Tensor
Tensor _std(const Tensor & self, bool unbiased); // aten::_std(Tensor self, bool unbiased=True) -> Tensor
std::tuple<Tensor,Tensor,Tensor> _svd_helper(const Tensor & self, bool some, bool compute_uv); // aten::_svd_helper(Tensor self, bool some, bool compute_uv) -> (Tensor, Tensor, Tensor)
std::tuple<Tensor,Tensor> _symeig_helper(const Tensor & self, bool eigenvectors, bool upper); // aten::_symeig_helper(Tensor self, bool eigenvectors, bool upper) -> (Tensor, Tensor)
std::tuple<Tensor,Tensor> _thnn_fused_gru_cell(const Tensor & input_gates, const Tensor & hidden_gates, const Tensor & hx, const Tensor & input_bias, const Tensor & hidden_bias); // aten::_thnn_fused_gru_cell(Tensor input_gates, Tensor hidden_gates, Tensor hx, Tensor? input_bias=None, Tensor? hidden_bias=None) -> (Tensor, Tensor)
std::tuple<Tensor,Tensor,Tensor,Tensor,Tensor> _thnn_fused_gru_cell_backward(const Tensor & grad_hy, const Tensor & workspace, bool has_bias); // aten::_thnn_fused_gru_cell_backward(Tensor grad_hy, Tensor workspace, bool has_bias) -> (Tensor, Tensor, Tensor, Tensor, Tensor)
std::tuple<Tensor,Tensor,Tensor> _thnn_fused_lstm_cell(const Tensor & input_gates, const Tensor & hidden_gates, const Tensor & cx, const Tensor & input_bias, const Tensor & hidden_bias); // aten::_thnn_fused_lstm_cell(Tensor input_gates, Tensor hidden_gates, Tensor cx, Tensor? input_bias=None, Tensor? hidden_bias=None) -> (Tensor, Tensor, Tensor)
std::tuple<Tensor,Tensor,Tensor,Tensor,Tensor> _thnn_fused_lstm_cell_backward(const Tensor & grad_hy, const Tensor & grad_cy, const Tensor & cx, const Tensor & cy, const Tensor & workspace, bool has_bias); // aten::_thnn_fused_lstm_cell_backward(Tensor? grad_hy, Tensor? grad_cy, Tensor cx, Tensor cy, Tensor workspace, bool has_bias) -> (Tensor, Tensor, Tensor, Tensor, Tensor)
std::tuple<Tensor,Tensor> _triangular_solve_helper(const Tensor & self, const Tensor & A, bool upper, bool transpose, bool unitriangular); // aten::_triangular_solve_helper(Tensor self, Tensor A, bool upper, bool transpose, bool unitriangular) -> (Tensor, Tensor)
Tensor _trilinear(const Tensor & i1, const Tensor & i2, const Tensor & i3, IntArrayRef expand1, IntArrayRef expand2, IntArrayRef expand3, IntArrayRef sumdim, int64_t unroll_dim); // aten::_trilinear(Tensor i1, Tensor i2, Tensor i3, int[] expand1, int[] expand2, int[] expand3, int[] sumdim, int unroll_dim=1) -> Tensor
std::tuple<Tensor,Tensor> _unique(const Tensor & self, bool sorted, bool return_inverse); // aten::_unique(Tensor self, bool sorted=True, bool return_inverse=False) -> (Tensor, Tensor)
std::tuple<Tensor,Tensor,Tensor> _unique2(const Tensor & self, bool sorted, bool return_inverse, bool return_counts); // aten::_unique2(Tensor self, bool sorted=True, bool return_inverse=False, bool return_counts=False) -> (Tensor, Tensor, Tensor)
Tensor _unsafe_view(const Tensor & self, IntArrayRef size); // aten::_unsafe_view(Tensor self, int[] size) -> Tensor
bool _use_cudnn_ctc_loss(const Tensor & log_probs, const Tensor & targets, IntArrayRef input_lengths, IntArrayRef target_lengths, int64_t blank); // aten::_use_cudnn_ctc_loss(Tensor log_probs, Tensor targets, int[] input_lengths, int[] target_lengths, int blank) -> bool
Tensor _values(const Tensor & self); // aten::_values(Tensor(a) self) -> Tensor(a)
Tensor _var(const Tensor & self, bool unbiased); // aten::_var(Tensor self, bool unbiased=True) -> Tensor
std::tuple<Tensor,Tensor> _weight_norm_cuda_interface(const Tensor & v, const Tensor & g, int64_t dim); // aten::_weight_norm_cuda_interface(Tensor v, Tensor g, int dim=0) -> (Tensor, Tensor)
std::tuple<Tensor,Tensor> _weight_norm_cuda_interface_backward(const Tensor & grad_w, const Tensor & saved_v, const Tensor & saved_g, const Tensor & saved_norms, int64_t dim); // aten::_weight_norm_cuda_interface_backward(Tensor grad_w, Tensor saved_v, Tensor saved_g, Tensor saved_norms, int dim) -> (Tensor, Tensor)
Tensor abs(const Tensor & self); // aten::abs(Tensor self) -> Tensor
Tensor & abs_(Tensor & self); // aten::abs_(Tensor(a!) self) -> Tensor(a!)
Tensor & abs_out(Tensor & out, const Tensor & self); // aten::abs.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)
Tensor acos(const Tensor & self); // aten::acos(Tensor self) -> Tensor
Tensor & acos_(Tensor & self); // aten::acos_(Tensor(a!) self) -> Tensor(a!)
Tensor & acos_out(Tensor & out, const Tensor & self); // aten::acos.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)
Tensor & adaptive_avg_pool2d_out(Tensor & out, const Tensor & self, IntArrayRef output_size); // aten::adaptive_avg_pool2d.out(Tensor self, int[2] output_size, *, Tensor(a!) out) -> Tensor(a!)
Tensor adaptive_avg_pool3d(const Tensor & self, IntArrayRef output_size); // aten::adaptive_avg_pool3d(Tensor self, int[3] output_size) -> Tensor
Tensor adaptive_avg_pool3d_backward(const Tensor & grad_output, const Tensor & self); // aten::adaptive_avg_pool3d_backward(Tensor grad_output, Tensor self) -> Tensor
Tensor & adaptive_avg_pool3d_backward_out(Tensor & grad_input, const Tensor & grad_output, const Tensor & self); // aten::adaptive_avg_pool3d_backward.grad_input(Tensor grad_output, Tensor self, *, Tensor(a!) grad_input) -> Tensor(a!)
Tensor & adaptive_avg_pool3d_out(Tensor & out, const Tensor & self, IntArrayRef output_size); // aten::adaptive_avg_pool3d.out(Tensor self, int[3] output_size, *, Tensor(a!) out) -> Tensor(a!)
std::tuple<Tensor,Tensor> adaptive_max_pool2d(const Tensor & self, IntArrayRef output_size); // aten::adaptive_max_pool2d(Tensor self, int[2] output_size) -> (Tensor, Tensor)
Tensor adaptive_max_pool2d_backward(const Tensor & grad_output, const Tensor & self, const Tensor & indices); // aten::adaptive_max_pool2d_backward(Tensor grad_output, Tensor self, Tensor indices) -> Tensor
Tensor & adaptive_max_pool2d_backward_out(Tensor & grad_input, const Tensor & grad_output, const Tensor & self, const Tensor & indices); // aten::adaptive_max_pool2d_backward.grad_input(Tensor grad_output, Tensor self, Tensor indices, *, Tensor(a!) grad_input) -> Tensor(a!)
std::tuple<Tensor &,Tensor &> adaptive_max_pool2d_out(Tensor & out, Tensor & indices, const Tensor & self, IntArrayRef output_size); // aten::adaptive_max_pool2d.out(Tensor self, int[2] output_size, *, Tensor(a!) out, Tensor(b!) indices) -> (Tensor(a!), Tensor(b!))
std::tuple<Tensor,Tensor> adaptive_max_pool3d(const Tensor & self, IntArrayRef output_size); // aten::adaptive_max_pool3d(Tensor self, int[3] output_size) -> (Tensor, Tensor)
Tensor adaptive_max_pool3d_backward(const Tensor & grad_output, const Tensor & self, const Tensor & indices); // aten::adaptive_max_pool3d_backward(Tensor grad_output, Tensor self, Tensor indices) -> Tensor
Tensor & adaptive_max_pool3d_backward_out(Tensor & grad_input, const Tensor & grad_output, const Tensor & self, const Tensor & indices); // aten::adaptive_max_pool3d_backward.grad_input(Tensor grad_output, Tensor self, Tensor indices, *, Tensor(a!) grad_input) -> Tensor(a!)
std::tuple<Tensor &,Tensor &> adaptive_max_pool3d_out(Tensor & out, Tensor & indices, const Tensor & self, IntArrayRef output_size); // aten::adaptive_max_pool3d.out(Tensor self, int[3] output_size, *, Tensor(a!) out, Tensor(b!) indices) -> (Tensor(a!), Tensor(b!))
Tensor add(const Tensor & self, const Tensor & other, Scalar alpha); // aten::add.Tensor(Tensor self, Tensor other, *, Scalar alpha=1) -> Tensor
Tensor add(const Tensor & self, Scalar other, Scalar alpha); // aten::add.Scalar(Tensor self, Scalar other, Scalar alpha=1) -> Tensor
Tensor & add_(Tensor & self, const Tensor & other, Scalar alpha); // aten::add_.Tensor(Tensor(a!) self, Tensor other, *, Scalar alpha=1) -> Tensor(a!)
Tensor & add_(Tensor & self, Scalar other, Scalar alpha); // aten::add_.Scalar(Tensor(a!) self, Scalar other, Scalar alpha=1) -> Tensor(a!)
Tensor & add_out(Tensor & out, const Tensor & self, const Tensor & other, Scalar alpha); // aten::add.out(Tensor self, Tensor other, *, Scalar alpha=1, Tensor(a!) out) -> Tensor(a!)
Tensor addbmm(const Tensor & self, const Tensor & batch1, const Tensor & batch2, Scalar beta, Scalar alpha); // aten::addbmm(Tensor self, Tensor batch1, Tensor batch2, *, Scalar beta=1, Scalar alpha=1) -> Tensor
Tensor & addbmm_(Tensor & self, const Tensor & batch1, const Tensor & batch2, Scalar beta, Scalar alpha); // aten::addbmm_(Tensor(a!) self, Tensor batch1, Tensor batch2, *, Scalar beta=1, Scalar alpha=1) -> Tensor(a!)
Tensor & addbmm_out(Tensor & out, const Tensor & self, const Tensor & batch1, const Tensor & batch2, Scalar beta, Scalar alpha); // aten::addbmm.out(Tensor self, Tensor batch1, Tensor batch2, *, Scalar beta=1, Scalar alpha=1, Tensor(a!) out) -> Tensor(a!)
Tensor addcdiv(const Tensor & self, const Tensor & tensor1, const Tensor & tensor2, Scalar value); // aten::addcdiv(Tensor self, Tensor tensor1, Tensor tensor2, *, Scalar value=1) -> Tensor
Tensor & addcdiv_(Tensor & self, const Tensor & tensor1, const Tensor & tensor2, Scalar value); // aten::addcdiv_(Tensor(a!) self, Tensor tensor1, Tensor tensor2, *, Scalar value=1) -> Tensor(a!)
Tensor & addcdiv_out(Tensor & out, const Tensor & self, const Tensor & tensor1, const Tensor & tensor2, Scalar value); // aten::addcdiv.out(Tensor self, Tensor tensor1, Tensor tensor2, *, Scalar value=1, Tensor(a!) out) -> Tensor(a!)
Tensor addcmul(const Tensor & self, const Tensor & tensor1, const Tensor & tensor2, Scalar value); // aten::addcmul(Tensor self, Tensor tensor1, Tensor tensor2, *, Scalar value=1) -> Tensor
Tensor & addcmul_(Tensor & self, const Tensor & tensor1, const Tensor & tensor2, Scalar value); // aten::addcmul_(Tensor(a!) self, Tensor tensor1, Tensor tensor2, *, Scalar value=1) -> Tensor(a!)
Tensor & addcmul_out(Tensor & out, const Tensor & self, const Tensor & tensor1, const Tensor & tensor2, Scalar value); // aten::addcmul.out(Tensor self, Tensor tensor1, Tensor tensor2, *, Scalar value=1, Tensor(a!) out) -> Tensor(a!)
Tensor addmm(const Tensor & self, const Tensor & mat1, const Tensor & mat2, Scalar beta, Scalar alpha); // aten::addmm(Tensor self, Tensor mat1, Tensor mat2, *, Scalar beta=1, Scalar alpha=1) -> Tensor
Tensor & addmm_(Tensor & self, const Tensor & mat1, const Tensor & mat2, Scalar beta, Scalar alpha); // aten::addmm_(Tensor(a!) self, Tensor mat1, Tensor mat2, *, Scalar beta=1, Scalar alpha=1) -> Tensor(a!)
Tensor & addmm_out(Tensor & out, const Tensor & self, const Tensor & mat1, const Tensor & mat2, Scalar beta, Scalar alpha); // aten::addmm.out(Tensor self, Tensor mat1, Tensor mat2, *, Scalar beta=1, Scalar alpha=1, Tensor(a!) out) -> Tensor(a!)
Tensor addmv(const Tensor & self, const Tensor & mat, const Tensor & vec, Scalar beta, Scalar alpha); // aten::addmv(Tensor self, Tensor mat, Tensor vec, *, Scalar beta=1, Scalar alpha=1) -> Tensor
Tensor & addmv_(Tensor & self, const Tensor & mat, const Tensor & vec, Scalar beta, Scalar alpha); // aten::addmv_(Tensor(a!) self, Tensor mat, Tensor vec, *, Scalar beta=1, Scalar alpha=1) -> Tensor(a!)
Tensor & addmv_out(Tensor & out, const Tensor & self, const Tensor & mat, const Tensor & vec, Scalar beta, Scalar alpha); // aten::addmv.out(Tensor self, Tensor mat, Tensor vec, *, Scalar beta=1, Scalar alpha=1, Tensor(a!) out) -> Tensor(a!)
Tensor addr(const Tensor & self, const Tensor & vec1, const Tensor & vec2, Scalar beta, Scalar alpha); // aten::addr(Tensor self, Tensor vec1, Tensor vec2, *, Scalar beta=1, Scalar alpha=1) -> Tensor
Tensor & addr_(Tensor & self, const Tensor & vec1, const Tensor & vec2, Scalar beta, Scalar alpha); // aten::addr_(Tensor(a!) self, Tensor vec1, Tensor vec2, *, Scalar beta=1, Scalar alpha=1) -> Tensor(a!)
Tensor & addr_out(Tensor & out, const Tensor & self, const Tensor & vec1, const Tensor & vec2, Scalar beta, Scalar alpha); // aten::addr.out(Tensor self, Tensor vec1, Tensor vec2, *, Scalar beta=1, Scalar alpha=1, Tensor(a!) out) -> Tensor(a!)
Tensor affine_grid_generator(const Tensor & theta, IntArrayRef size, bool align_corners); // aten::affine_grid_generator(Tensor theta, int[] size, bool align_corners) -> Tensor
Tensor alias(const Tensor & self); // aten::alias(Tensor(a) self) -> Tensor(a)
Tensor all(const Tensor & self, int64_t dim, bool keepdim); // aten::all.dim(Tensor self, int dim, bool keepdim=False) -> Tensor
Tensor all(const Tensor & self); // aten::all(Tensor self) -> Tensor
Tensor & all_out(Tensor & out, const Tensor & self, int64_t dim, bool keepdim); // aten::all.out(Tensor self, int dim, bool keepdim=False, *, Tensor(a!) out) -> Tensor(a!)
Tensor angle(const Tensor & self); // aten::angle(Tensor self) -> Tensor
Tensor & angle_out(Tensor & out, const Tensor & self); // aten::angle.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)
Tensor any(const Tensor & self, int64_t dim, bool keepdim); // aten::any.dim(Tensor self, int dim, bool keepdim=False) -> Tensor
Tensor any(const Tensor & self); // aten::any(Tensor self) -> Tensor
Tensor & any_out(Tensor & out, const Tensor & self, int64_t dim, bool keepdim); // aten::any.out(Tensor self, int dim, bool keepdim=False, *, Tensor(a!) out) -> Tensor(a!)
Tensor & arange_out(Tensor & out, Scalar start, Scalar end, Scalar step); // aten::arange.start_out(Scalar start, Scalar end, Scalar step=1, *, Tensor(a!) out) -> Tensor(a!)
Tensor as_strided(const Tensor & self, IntArrayRef size, IntArrayRef stride, c10::optional<int64_t> storage_offset); // aten::as_strided(Tensor(a) self, int[] size, int[] stride, int? storage_offset=None) -> Tensor(a)
Tensor & as_strided_(Tensor & self, IntArrayRef size, IntArrayRef stride, c10::optional<int64_t> storage_offset); // aten::as_strided_(Tensor(a!) self, int[] size, int[] stride, int? storage_offset=None) -> Tensor(a!)
Tensor asin(const Tensor & self); // aten::asin(Tensor self) -> Tensor
Tensor & asin_(Tensor & self); // aten::asin_(Tensor(a!) self) -> Tensor(a!)
Tensor & asin_out(Tensor & out, const Tensor & self); // aten::asin.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)
Tensor atan(const Tensor & self); // aten::atan(Tensor self) -> Tensor
Tensor atan2(const Tensor & self, const Tensor & other); // aten::atan2(Tensor self, Tensor other) -> Tensor
Tensor & atan2_(Tensor & self, const Tensor & other); // aten::atan2_(Tensor(a!) self, Tensor other) -> Tensor(a!)
Tensor & atan2_out(Tensor & out, const Tensor & self, const Tensor & other); // aten::atan2.out(Tensor self, Tensor other, *, Tensor(a!) out) -> Tensor(a!)
Tensor & atan_(Tensor & self); // aten::atan_(Tensor(a!) self) -> Tensor(a!)
Tensor & atan_out(Tensor & out, const Tensor & self); // aten::atan.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)
Tensor avg_pool2d(const Tensor & self, IntArrayRef kernel_size, IntArrayRef stride, IntArrayRef padding, bool ceil_mode, bool count_include_pad, c10::optional<int64_t> divisor_override); // aten::avg_pool2d(Tensor self, int[2] kernel_size, int[2] stride=[], int[2] padding=0, bool ceil_mode=False, bool count_include_pad=True, int? divisor_override=None) -> Tensor
Tensor avg_pool2d_backward(const Tensor & grad_output, const Tensor & self, IntArrayRef kernel_size, IntArrayRef stride, IntArrayRef padding, bool ceil_mode, bool count_include_pad, c10::optional<int64_t> divisor_override); // aten::avg_pool2d_backward(Tensor grad_output, Tensor self, int[2] kernel_size, int[2] stride, int[2] padding, bool ceil_mode, bool count_include_pad, int? divisor_override) -> Tensor
Tensor & avg_pool2d_backward_out(Tensor & grad_input, const Tensor & grad_output, const Tensor & self, IntArrayRef kernel_size, IntArrayRef stride, IntArrayRef padding, bool ceil_mode, bool count_include_pad, c10::optional<int64_t> divisor_override); // aten::avg_pool2d_backward.grad_input(Tensor grad_output, Tensor self, int[2] kernel_size, int[2] stride, int[2] padding, bool ceil_mode, bool count_include_pad, int? divisor_override, *, Tensor(a!) grad_input) -> Tensor(a!)
Tensor & avg_pool2d_out(Tensor & out, const Tensor & self, IntArrayRef kernel_size, IntArrayRef stride, IntArrayRef padding, bool ceil_mode, bool count_include_pad, c10::optional<int64_t> divisor_override); // aten::avg_pool2d.out(Tensor self, int[2] kernel_size, int[2] stride=[], int[2] padding=0, bool ceil_mode=False, bool count_include_pad=True, int? divisor_override=None, *, Tensor(a!) out) -> Tensor(a!)
Tensor avg_pool3d(const Tensor & self, IntArrayRef kernel_size, IntArrayRef stride, IntArrayRef padding, bool ceil_mode, bool count_include_pad, c10::optional<int64_t> divisor_override); // aten::avg_pool3d(Tensor self, int[3] kernel_size, int[3] stride=[], int[3] padding=0, bool ceil_mode=False, bool count_include_pad=True, int? divisor_override=None) -> Tensor
Tensor avg_pool3d_backward(const Tensor & grad_output, const Tensor & self, IntArrayRef kernel_size, IntArrayRef stride, IntArrayRef padding, bool ceil_mode, bool count_include_pad, c10::optional<int64_t> divisor_override); // aten::avg_pool3d_backward(Tensor grad_output, Tensor self, int[3] kernel_size, int[3] stride, int[3] padding, bool ceil_mode, bool count_include_pad, int? divisor_override) -> Tensor
Tensor & avg_pool3d_backward_out(Tensor & grad_input, const Tensor & grad_output, const Tensor & self, IntArrayRef kernel_size, IntArrayRef stride, IntArrayRef padding, bool ceil_mode, bool count_include_pad, c10::optional<int64_t> divisor_override); // aten::avg_pool3d_backward.grad_input(Tensor grad_output, Tensor self, int[3] kernel_size, int[3] stride, int[3] padding, bool ceil_mode, bool count_include_pad, int? divisor_override, *, Tensor(a!) grad_input) -> Tensor(a!)
Tensor & avg_pool3d_out(Tensor & out, const Tensor & self, IntArrayRef kernel_size, IntArrayRef stride, IntArrayRef padding, bool ceil_mode, bool count_include_pad, c10::optional<int64_t> divisor_override); // aten::avg_pool3d.out(Tensor self, int[3] kernel_size, int[3] stride=[], int[3] padding=0, bool ceil_mode=False, bool count_include_pad=True, int? divisor_override=None, *, Tensor(a!) out) -> Tensor(a!)
Tensor baddbmm(const Tensor & self, const Tensor & batch1, const Tensor & batch2, Scalar beta, Scalar alpha); // aten::baddbmm(Tensor self, Tensor batch1, Tensor batch2, *, Scalar beta=1, Scalar alpha=1) -> Tensor
Tensor & baddbmm_(Tensor & self, const Tensor & batch1, const Tensor & batch2, Scalar beta, Scalar alpha); // aten::baddbmm_(Tensor(a!) self, Tensor batch1, Tensor batch2, *, Scalar beta=1, Scalar alpha=1) -> Tensor(a!)
Tensor & baddbmm_out(Tensor & out, const Tensor & self, const Tensor & batch1, const Tensor & batch2, Scalar beta, Scalar alpha); // aten::baddbmm.out(Tensor self, Tensor batch1, Tensor batch2, *, Scalar beta=1, Scalar alpha=1, Tensor(a!) out) -> Tensor(a!)
Tensor batch_norm_backward_elemt(const Tensor & grad_out, const Tensor & input, const Tensor & mean, const Tensor & invstd, const Tensor & weight, const Tensor & mean_dy, const Tensor & mean_dy_xmu); // aten::batch_norm_backward_elemt(Tensor grad_out, Tensor input, Tensor mean, Tensor invstd, Tensor? weight, Tensor mean_dy, Tensor mean_dy_xmu) -> Tensor
std::tuple<Tensor,Tensor,Tensor,Tensor> batch_norm_backward_reduce(const Tensor & grad_out, const Tensor & input, const Tensor & mean, const Tensor & invstd, const Tensor & weight, bool input_g, bool weight_g, bool bias_g); // aten::batch_norm_backward_reduce(Tensor grad_out, Tensor input, Tensor mean, Tensor invstd, Tensor? weight, bool input_g, bool weight_g, bool bias_g) -> (Tensor, Tensor, Tensor, Tensor)
Tensor batch_norm_elemt(const Tensor & input, const Tensor & weight, const Tensor & bias, const Tensor & mean, const Tensor & invstd, double eps); // aten::batch_norm_elemt(Tensor input, Tensor? weight, Tensor? bias, Tensor mean, Tensor invstd, float eps) -> Tensor
Tensor & batch_norm_elemt_out(Tensor & out, const Tensor & input, const Tensor & weight, const Tensor & bias, const Tensor & mean, const Tensor & invstd, double eps); // aten::batch_norm_elemt.out(Tensor input, Tensor? weight, Tensor? bias, Tensor mean, Tensor invstd, float eps, *, Tensor(a!) out) -> Tensor(a!)
std::tuple<Tensor,Tensor> batch_norm_gather_stats(const Tensor & input, const Tensor & mean, const Tensor & invstd, const Tensor & running_mean, const Tensor & running_var, double momentum, double eps, int64_t count); // aten::batch_norm_gather_stats(Tensor input, Tensor mean, Tensor invstd, Tensor? running_mean, Tensor? running_var, float momentum, float eps, int count) -> (Tensor, Tensor)
std::tuple<Tensor,Tensor> batch_norm_gather_stats_with_counts(const Tensor & input, const Tensor & mean, const Tensor & invstd, const Tensor & running_mean, const Tensor & running_var, double momentum, double eps, IntArrayRef counts); // aten::batch_norm_gather_stats_with_counts(Tensor input, Tensor mean, Tensor invstd, Tensor? running_mean, Tensor? running_var, float momentum, float eps, int[] counts) -> (Tensor, Tensor)
std::tuple<Tensor,Tensor> batch_norm_stats(const Tensor & input, double eps); // aten::batch_norm_stats(Tensor input, float eps) -> (Tensor, Tensor)
std::tuple<Tensor,Tensor> batch_norm_update_stats(const Tensor & input, const Tensor & running_mean, const Tensor & running_var, double momentum); // aten::batch_norm_update_stats(Tensor input, Tensor? running_mean, Tensor? running_var, float momentum) -> (Tensor, Tensor)
Tensor bernoulli(const Tensor & self, Generator * generator); // aten::bernoulli(Tensor self, *, Generator? generator=None) -> Tensor
Tensor & bernoulli_(Tensor & self, const Tensor & p, Generator * generator); // aten::bernoulli_.Tensor(Tensor(a!) self, Tensor p, *, Generator? generator=None) -> Tensor(a!)
Tensor & bernoulli_(Tensor & self, double p, Generator * generator); // aten::bernoulli_.float(Tensor(a!) self, float p=0.5, *, Generator? generator=None) -> Tensor(a!)
Tensor & bernoulli_out(Tensor & out, const Tensor & self, Generator * generator); // aten::bernoulli.out(Tensor self, *, Generator? generator=None, Tensor(a!) out) -> Tensor(a!)
Tensor binary_cross_entropy(const Tensor & self, const Tensor & target, const Tensor & weight, int64_t reduction); // aten::binary_cross_entropy(Tensor self, Tensor target, Tensor? weight=None, int reduction=Mean) -> Tensor
Tensor binary_cross_entropy_backward(const Tensor & grad_output, const Tensor & self, const Tensor & target, const Tensor & weight, int64_t reduction); // aten::binary_cross_entropy_backward(Tensor grad_output, Tensor self, Tensor target, Tensor? weight=None, int reduction=Mean) -> Tensor
Tensor & binary_cross_entropy_backward_out(Tensor & grad_input, const Tensor & grad_output, const Tensor & self, const Tensor & target, const Tensor & weight, int64_t reduction); // aten::binary_cross_entropy_backward.grad_input(Tensor grad_output, Tensor self, Tensor target, Tensor? weight=None, int reduction=Mean, *, Tensor(a!) grad_input) -> Tensor(a!)
Tensor & binary_cross_entropy_out(Tensor & out, const Tensor & self, const Tensor & target, const Tensor & weight, int64_t reduction); // aten::binary_cross_entropy.out(Tensor self, Tensor target, Tensor? weight=None, int reduction=Mean, *, Tensor(a!) out) -> Tensor(a!)
Tensor binary_cross_entropy_with_logits(const Tensor & self, const Tensor & target, const Tensor & weight, const Tensor & pos_weight, int64_t reduction); // aten::binary_cross_entropy_with_logits(Tensor self, Tensor target, Tensor? weight=None, Tensor? pos_weight=None, int reduction=Mean) -> Tensor
Tensor bincount(const Tensor & self, const Tensor & weights, int64_t minlength); // aten::bincount(Tensor self, Tensor? weights=None, int minlength=0) -> Tensor
Tensor & bitwise_not_out(Tensor & out, const Tensor & self); // aten::bitwise_not.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)
Tensor & bitwise_xor_out(Tensor & out, const Tensor & self, const Tensor & other); // aten::bitwise_xor.Tensor_out(Tensor self, Tensor other, *, Tensor(a!) out) -> Tensor(a!)
Tensor & bitwise_xor_out(Tensor & out, const Tensor & self, Scalar other); // aten::bitwise_xor.Scalar_out(Tensor self, Scalar other, *, Tensor(a!) out) -> Tensor(a!)
Tensor bmm(const Tensor & self, const Tensor & mat2); // aten::bmm(Tensor self, Tensor mat2) -> Tensor
Tensor & bmm_out(Tensor & out, const Tensor & self, const Tensor & mat2); // aten::bmm.out(Tensor self, Tensor mat2, *, Tensor(a!) out) -> Tensor(a!)
Tensor cat(TensorList tensors, int64_t dim); // aten::cat(Tensor[] tensors, int dim=0) -> Tensor
Tensor & cat_out(Tensor & out, TensorList tensors, int64_t dim); // aten::cat.out(Tensor[] tensors, int dim=0, *, Tensor(a!) out) -> Tensor(a!)
Tensor & cauchy_(Tensor & self, double median, double sigma, Generator * generator); // aten::cauchy_(Tensor(a!) self, float median=0, float sigma=1, *, Generator? generator=None) -> Tensor(a!)
Tensor cdist(const Tensor & x1, const Tensor & x2, double p, c10::optional<int64_t> compute_mode); // aten::cdist(Tensor x1, Tensor x2, float p=2, int? compute_mode=None) -> Tensor
Tensor ceil(const Tensor & self); // aten::ceil(Tensor self) -> Tensor
Tensor & ceil_(Tensor & self); // aten::ceil_(Tensor(a!) self) -> Tensor(a!)
Tensor & ceil_out(Tensor & out, const Tensor & self); // aten::ceil.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)
Tensor cholesky(const Tensor & self, bool upper); // aten::cholesky(Tensor self, bool upper=False) -> Tensor
Tensor cholesky_inverse(const Tensor & self, bool upper); // aten::cholesky_inverse(Tensor self, bool upper=False) -> Tensor
Tensor & cholesky_inverse_out(Tensor & out, const Tensor & self, bool upper); // aten::cholesky_inverse.out(Tensor self, bool upper=False, *, Tensor(a!) out) -> Tensor(a!)
Tensor & cholesky_out(Tensor & out, const Tensor & self, bool upper); // aten::cholesky.out(Tensor self, bool upper=False, *, Tensor(a!) out) -> Tensor(a!)
Tensor cholesky_solve(const Tensor & self, const Tensor & input2, bool upper); // aten::cholesky_solve(Tensor self, Tensor input2, bool upper=False) -> Tensor
Tensor & cholesky_solve_out(Tensor & out, const Tensor & self, const Tensor & input2, bool upper); // aten::cholesky_solve.out(Tensor self, Tensor input2, bool upper=False, *, Tensor(a!) out) -> Tensor(a!)
Tensor clamp(const Tensor & self, c10::optional<Scalar> min, c10::optional<Scalar> max); // aten::clamp(Tensor self, Scalar? min=None, Scalar? max=None) -> Tensor
Tensor & clamp_(Tensor & self, c10::optional<Scalar> min, c10::optional<Scalar> max); // aten::clamp_(Tensor(a!) self, Scalar? min=None, Scalar? max=None) -> Tensor(a!)
Tensor clamp_max(const Tensor & self, Scalar max); // aten::clamp_max(Tensor self, Scalar max) -> Tensor
Tensor & clamp_max_(Tensor & self, Scalar max); // aten::clamp_max_(Tensor(a!) self, Scalar max) -> Tensor(a!)
Tensor & clamp_max_out(Tensor & out, const Tensor & self, Scalar max); // aten::clamp_max.out(Tensor self, Scalar max, *, Tensor(a!) out) -> Tensor(a!)
Tensor clamp_min(const Tensor & self, Scalar min); // aten::clamp_min(Tensor self, Scalar min) -> Tensor
Tensor & clamp_min_(Tensor & self, Scalar min); // aten::clamp_min_(Tensor(a!) self, Scalar min) -> Tensor(a!)
Tensor & clamp_min_out(Tensor & out, const Tensor & self, Scalar min); // aten::clamp_min.out(Tensor self, Scalar min, *, Tensor(a!) out) -> Tensor(a!)
Tensor & clamp_out(Tensor & out, const Tensor & self, c10::optional<Scalar> min, c10::optional<Scalar> max); // aten::clamp.out(Tensor self, Scalar? min=None, Scalar? max=None, *, Tensor(a!) out) -> Tensor(a!)
Tensor clone(const Tensor & self, c10::optional<MemoryFormat> memory_format); // aten::clone(Tensor self, *, MemoryFormat? memory_format=None) -> Tensor
Tensor coalesce(const Tensor & self); // aten::coalesce(Tensor self) -> Tensor
Tensor col2im(const Tensor & self, IntArrayRef output_size, IntArrayRef kernel_size, IntArrayRef dilation, IntArrayRef padding, IntArrayRef stride); // aten::col2im(Tensor self, int[2] output_size, int[2] kernel_size, int[2] dilation, int[2] padding, int[2] stride) -> Tensor
Tensor col2im_backward(const Tensor & grad_output, IntArrayRef kernel_size, IntArrayRef dilation, IntArrayRef padding, IntArrayRef stride); // aten::col2im_backward(Tensor grad_output, int[2] kernel_size, int[2] dilation, int[2] padding, int[2] stride) -> Tensor
Tensor & col2im_backward_out(Tensor & grad_input, const Tensor & grad_output, IntArrayRef kernel_size, IntArrayRef dilation, IntArrayRef padding, IntArrayRef stride); // aten::col2im_backward.grad_input(Tensor grad_output, int[2] kernel_size, int[2] dilation, int[2] padding, int[2] stride, *, Tensor(a!) grad_input) -> Tensor(a!)
Tensor & col2im_out(Tensor & out, const Tensor & self, IntArrayRef output_size, IntArrayRef kernel_size, IntArrayRef dilation, IntArrayRef padding, IntArrayRef stride); // aten::col2im.out(Tensor self, int[2] output_size, int[2] kernel_size, int[2] dilation, int[2] padding, int[2] stride, *, Tensor(a!) out) -> Tensor(a!)
Tensor conj(const Tensor & self); // aten::conj(Tensor self) -> Tensor
Tensor & conj_out(Tensor & out, const Tensor & self); // aten::conj.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)
Tensor constant_pad_nd(const Tensor & self, IntArrayRef pad, Scalar value); // aten::constant_pad_nd(Tensor self, int[] pad, Scalar value=0) -> Tensor
Tensor conv_tbc(const Tensor & self, const Tensor & weight, const Tensor & bias, int64_t pad); // aten::conv_tbc(Tensor self, Tensor weight, Tensor bias, int pad=0) -> Tensor
std::tuple<Tensor,Tensor,Tensor> convolution_backward_overrideable(const Tensor & grad_output, const Tensor & input, const Tensor & weight, IntArrayRef stride, IntArrayRef padding, IntArrayRef dilation, bool transposed, IntArrayRef output_padding, int64_t groups, std::array<bool,3> output_mask); // aten::convolution_backward_overrideable(Tensor grad_output, Tensor input, Tensor weight, int[] stride, int[] padding, int[] dilation, bool transposed, int[] output_padding, int groups, bool[3] output_mask) -> (Tensor grad_input, Tensor grad_weight, Tensor grad_bias)
Tensor convolution_overrideable(const Tensor & input, const Tensor & weight, const Tensor & bias, IntArrayRef stride, IntArrayRef padding, IntArrayRef dilation, bool transposed, IntArrayRef output_padding, int64_t groups); // aten::convolution_overrideable(Tensor input, Tensor weight, Tensor? bias, int[] stride, int[] padding, int[] dilation, bool transposed, int[] output_padding, int groups) -> Tensor
Tensor & copy_(Tensor & self, const Tensor & src, bool non_blocking); // aten::copy_(Tensor(a!) self, Tensor src, bool non_blocking=False) -> Tensor(a!)
Tensor & copy_sparse_to_sparse_(Tensor & self, const Tensor & src, bool non_blocking); // aten::copy_sparse_to_sparse_(Tensor(a!) self, Tensor src, bool non_blocking=False) -> Tensor(a!)
Tensor cos(const Tensor & self); // aten::cos(Tensor self) -> Tensor
Tensor & cos_(Tensor & self); // aten::cos_(Tensor(a!) self) -> Tensor(a!)
Tensor & cos_out(Tensor & out, const Tensor & self); // aten::cos.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)
Tensor cosh(const Tensor & self); // aten::cosh(Tensor self) -> Tensor
Tensor & cosh_(Tensor & self); // aten::cosh_(Tensor(a!) self) -> Tensor(a!)
Tensor & cosh_out(Tensor & out, const Tensor & self); // aten::cosh.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)
Tensor cross(const Tensor & self, const Tensor & other, c10::optional<int64_t> dim); // aten::cross(Tensor self, Tensor other, int? dim=None) -> Tensor
Tensor & cross_out(Tensor & out, const Tensor & self, const Tensor & other, c10::optional<int64_t> dim); // aten::cross.out(Tensor self, Tensor other, int? dim=None, *, Tensor(a!) out) -> Tensor(a!)
Tensor cudnn_affine_grid_generator(const Tensor & theta, int64_t N, int64_t C, int64_t H, int64_t W); // aten::cudnn_affine_grid_generator(Tensor theta, int N, int C, int H, int W) -> Tensor grid
Tensor cudnn_affine_grid_generator_backward(const Tensor & grad, int64_t N, int64_t C, int64_t H, int64_t W); // aten::cudnn_affine_grid_generator_backward(Tensor grad, int N, int C, int H, int W) -> Tensor grad_theta
std::tuple<Tensor,Tensor,Tensor,Tensor> cudnn_batch_norm(const Tensor & input, const Tensor & weight, const Tensor & bias, const Tensor & running_mean, const Tensor & running_var, bool training, double exponential_average_factor, double epsilon); // aten::cudnn_batch_norm(Tensor input, Tensor weight, Tensor? bias, Tensor? running_mean, Tensor? running_var, bool training, float exponential_average_factor, float epsilon) -> (Tensor, Tensor, Tensor, Tensor)
std::tuple<Tensor,Tensor,Tensor> cudnn_batch_norm_backward(const Tensor & input, const Tensor & grad_output, const Tensor & weight, const Tensor & running_mean, const Tensor & running_var, const Tensor & save_mean, const Tensor & save_var, double epsilon, const Tensor & reserveSpace); // aten::cudnn_batch_norm_backward(Tensor input, Tensor grad_output, Tensor weight, Tensor? running_mean, Tensor? running_var, Tensor? save_mean, Tensor? save_var, float epsilon, Tensor reserveSpace) -> (Tensor, Tensor, Tensor)
Tensor cudnn_convolution(const Tensor & self, const Tensor & weight, const Tensor & bias, IntArrayRef padding, IntArrayRef stride, IntArrayRef dilation, int64_t groups, bool benchmark, bool deterministic); // aten::cudnn_convolution(Tensor self, Tensor weight, Tensor? bias, int[] padding, int[] stride, int[] dilation, int groups, bool benchmark, bool deterministic) -> Tensor
std::tuple<Tensor,Tensor,Tensor> cudnn_convolution_backward(const Tensor & self, const Tensor & grad_output, const Tensor & weight, IntArrayRef padding, IntArrayRef stride, IntArrayRef dilation, int64_t groups, bool benchmark, bool deterministic, std::array<bool,3> output_mask); // aten::cudnn_convolution_backward(Tensor self, Tensor grad_output, Tensor weight, int[] padding, int[] stride, int[] dilation, int groups, bool benchmark, bool deterministic, bool[3] output_mask) -> (Tensor, Tensor, Tensor)
Tensor cudnn_convolution_backward_bias(const Tensor & grad_output); // aten::cudnn_convolution_backward_bias(Tensor grad_output) -> Tensor
Tensor cudnn_convolution_backward_input(IntArrayRef self_size, const Tensor & grad_output, const Tensor & weight, IntArrayRef padding, IntArrayRef stride, IntArrayRef dilation, int64_t groups, bool benchmark, bool deterministic); // aten::cudnn_convolution_backward_input(int[] self_size, Tensor grad_output, Tensor weight, int[] padding, int[] stride, int[] dilation, int groups, bool benchmark, bool deterministic) -> Tensor
Tensor cudnn_convolution_backward_weight(IntArrayRef weight_size, const Tensor & grad_output, const Tensor & self, IntArrayRef padding, IntArrayRef stride, IntArrayRef dilation, int64_t groups, bool benchmark, bool deterministic); // aten::cudnn_convolution_backward_weight(int[] weight_size, Tensor grad_output, Tensor self, int[] padding, int[] stride, int[] dilation, int groups, bool benchmark, bool deterministic) -> Tensor
Tensor cudnn_convolution_transpose(const Tensor & self, const Tensor & weight, const Tensor & bias, IntArrayRef padding, IntArrayRef output_padding, IntArrayRef stride, IntArrayRef dilation, int64_t groups, bool benchmark, bool deterministic); // aten::cudnn_convolution_transpose(Tensor self, Tensor weight, Tensor? bias, int[] padding, int[] output_padding, int[] stride, int[] dilation, int groups, bool benchmark, bool deterministic) -> Tensor
std::tuple<Tensor,Tensor,Tensor> cudnn_convolution_transpose_backward(const Tensor & self, const Tensor & grad_output, const Tensor & weight, IntArrayRef padding, IntArrayRef output_padding, IntArrayRef stride, IntArrayRef dilation, int64_t groups, bool benchmark, bool deterministic, std::array<bool,3> output_mask); // aten::cudnn_convolution_transpose_backward(Tensor self, Tensor grad_output, Tensor weight, int[] padding, int[] output_padding, int[] stride, int[] dilation, int groups, bool benchmark, bool deterministic, bool[3] output_mask) -> (Tensor, Tensor, Tensor)
Tensor cudnn_convolution_transpose_backward_bias(const Tensor & grad_output); // aten::cudnn_convolution_transpose_backward_bias(Tensor grad_output) -> Tensor
Tensor cudnn_convolution_transpose_backward_input(const Tensor & grad_output, const Tensor & weight, IntArrayRef padding, IntArrayRef stride, IntArrayRef dilation, int64_t groups, bool benchmark, bool deterministic); // aten::cudnn_convolution_transpose_backward_input(Tensor grad_output, Tensor weight, int[] padding, int[] stride, int[] dilation, int groups, bool benchmark, bool deterministic) -> Tensor
Tensor cudnn_convolution_transpose_backward_weight(IntArrayRef weight_size, const Tensor & grad_output, const Tensor & self, IntArrayRef padding, IntArrayRef stride, IntArrayRef dilation, int64_t groups, bool benchmark, bool deterministic); // aten::cudnn_convolution_transpose_backward_weight(int[] weight_size, Tensor grad_output, Tensor self, int[] padding, int[] stride, int[] dilation, int groups, bool benchmark, bool deterministic) -> Tensor
Tensor cudnn_grid_sampler(const Tensor & self, const Tensor & grid); // aten::cudnn_grid_sampler(Tensor self, Tensor grid) -> Tensor output
std::tuple<Tensor,Tensor> cudnn_grid_sampler_backward(const Tensor & self, const Tensor & grid, const Tensor & grad_output); // aten::cudnn_grid_sampler_backward(Tensor self, Tensor grid, Tensor grad_output) -> (Tensor grad_self, Tensor grad_grid)
Tensor cumprod(const Tensor & self, int64_t dim, c10::optional<ScalarType> dtype); // aten::cumprod(Tensor self, int dim, *, ScalarType? dtype=None) -> Tensor
Tensor & cumprod_out(Tensor & out, const Tensor & self, int64_t dim, c10::optional<ScalarType> dtype); // aten::cumprod.out(Tensor self, int dim, *, ScalarType? dtype=None, Tensor(a!) out) -> Tensor(a!)
Tensor cumsum(const Tensor & self, int64_t dim, c10::optional<ScalarType> dtype); // aten::cumsum(Tensor self, int dim, *, ScalarType? dtype=None) -> Tensor
Tensor & cumsum_out(Tensor & out, const Tensor & self, int64_t dim, c10::optional<ScalarType> dtype); // aten::cumsum.out(Tensor self, int dim, *, ScalarType? dtype=None, Tensor(a!) out) -> Tensor(a!)
int64_t dense_dim(const Tensor & self); // aten::dense_dim(Tensor self) -> int
Tensor dequantize(const Tensor & self); // aten::dequantize(Tensor self) -> Tensor
Tensor det(const Tensor & self); // aten::det(Tensor self) -> Tensor
Tensor diag(const Tensor & self, int64_t diagonal); // aten::diag(Tensor self, int diagonal=0) -> Tensor
Tensor & diag_out(Tensor & out, const Tensor & self, int64_t diagonal); // aten::diag.out(Tensor self, int diagonal=0, *, Tensor(a!) out) -> Tensor(a!)
Tensor diagonal(const Tensor & self, int64_t offset, int64_t dim1, int64_t dim2); // aten::diagonal(Tensor(a) self, int offset=0, int dim1=0, int dim2=1) -> Tensor(a)
Tensor digamma(const Tensor & self); // aten::digamma(Tensor self) -> Tensor
Tensor & digamma_(Tensor & self); // aten::digamma_(Tensor(a!) self) -> Tensor(a!)
Tensor & digamma_out(Tensor & out, const Tensor & self); // aten::digamma.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)
Tensor dist(const Tensor & self, const Tensor & other, Scalar p); // aten::dist(Tensor self, Tensor other, Scalar p=2) -> Tensor
Tensor div(const Tensor & self, const Tensor & other); // aten::div.Tensor(Tensor self, Tensor other) -> Tensor
Tensor div(const Tensor & self, Scalar other); // aten::div.Scalar(Tensor self, Scalar other) -> Tensor
Tensor & div_(Tensor & self, const Tensor & other); // aten::div_.Tensor(Tensor(a!) self, Tensor other) -> Tensor(a!)
Tensor & div_(Tensor & self, Scalar other); // aten::div_.Scalar(Tensor(a!) self, Scalar other) -> Tensor(a!)
Tensor & div_out(Tensor & out, const Tensor & self, const Tensor & other); // aten::div.out(Tensor self, Tensor other, *, Tensor(a!) out) -> Tensor(a!)
Tensor dot(const Tensor & self, const Tensor & tensor); // aten::dot(Tensor self, Tensor tensor) -> Tensor
Tensor & dot_out(Tensor & out, const Tensor & self, const Tensor & tensor); // aten::dot.out(Tensor self, Tensor tensor, *, Tensor(a!) out) -> Tensor(a!)
std::tuple<Tensor,Tensor> eig(const Tensor & self, bool eigenvectors); // aten::eig(Tensor self, bool eigenvectors=False) -> (Tensor eigenvalues, Tensor eigenvectors)
std::tuple<Tensor &,Tensor &> eig_out(Tensor & e, Tensor & v, const Tensor & self, bool eigenvectors); // aten::eig.e(Tensor self, bool eigenvectors=False, *, Tensor(a!) e, Tensor(b!) v) -> (Tensor(a!) eigenvalues, Tensor(b!) eigenvectors)
Tensor einsum(std::string equation, TensorList tensors); // aten::einsum(str equation, Tensor[] tensors) -> Tensor
Tensor elu(const Tensor & self, Scalar alpha, Scalar scale, Scalar input_scale); // aten::elu(Tensor self, Scalar alpha=1, Scalar scale=1, Scalar input_scale=1) -> Tensor
Tensor & elu_(Tensor & self, Scalar alpha, Scalar scale, Scalar input_scale); // aten::elu_(Tensor(a!) self, Scalar alpha=1, Scalar scale=1, Scalar input_scale=1) -> Tensor(a!)
Tensor elu_backward(const Tensor & grad_output, Scalar alpha, Scalar scale, Scalar input_scale, const Tensor & output); // aten::elu_backward(Tensor grad_output, Scalar alpha, Scalar scale, Scalar input_scale, Tensor output) -> Tensor
Tensor & elu_backward_out(Tensor & grad_input, const Tensor & grad_output, Scalar alpha, Scalar scale, Scalar input_scale, const Tensor & output); // aten::elu_backward.grad_input(Tensor grad_output, Scalar alpha, Scalar scale, Scalar input_scale, Tensor output, *, Tensor(a!) grad_input) -> Tensor(a!)
Tensor & elu_out(Tensor & out, const Tensor & self, Scalar alpha, Scalar scale, Scalar input_scale); // aten::elu.out(Tensor self, Scalar alpha=1, Scalar scale=1, Scalar input_scale=1, *, Tensor(a!) out) -> Tensor(a!)
Tensor embedding(const Tensor & weight, const Tensor & indices, int64_t padding_idx, bool scale_grad_by_freq, bool sparse); // aten::embedding(Tensor weight, Tensor indices, int padding_idx=-1, bool scale_grad_by_freq=False, bool sparse=False) -> Tensor
Tensor embedding_dense_backward(const Tensor & grad_output, const Tensor & indices, int64_t num_weights, int64_t padding_idx, bool scale_grad_by_freq); // aten::embedding_dense_backward(Tensor grad_output, Tensor indices, int num_weights, int padding_idx, bool scale_grad_by_freq) -> Tensor
Tensor & embedding_renorm_(Tensor & self, const Tensor & indices, double max_norm, double norm_type); // aten::embedding_renorm_(Tensor(a!) self, Tensor indices, float max_norm, float norm_type) -> Tensor(a!)
Tensor empty(IntArrayRef size, const TensorOptions & options, c10::optional<MemoryFormat> memory_format); // aten::empty.memory_format(int[] size, *, ScalarType? dtype=None, Layout? layout=None, Device? device=None, bool? pin_memory=None, MemoryFormat? memory_format=None) -> Tensor
Tensor empty_strided(IntArrayRef size, IntArrayRef stride, const TensorOptions & options); // aten::empty_strided(int[] size, int[] stride, *, ScalarType? dtype=None, Layout? layout=None, Device? device=None, bool? pin_memory=None) -> Tensor
Tensor eq(const Tensor & self, Scalar other); // aten::eq.Scalar(Tensor self, Scalar other) -> Tensor
Tensor eq(const Tensor & self, const Tensor & other); // aten::eq.Tensor(Tensor self, Tensor other) -> Tensor
Tensor & eq_(Tensor & self, Scalar other); // aten::eq_.Scalar(Tensor(a!) self, Scalar other) -> Tensor(a!)
Tensor & eq_(Tensor & self, const Tensor & other); // aten::eq_.Tensor(Tensor(a!) self, Tensor other) -> Tensor(a!)
Tensor & eq_out(Tensor & out, const Tensor & self, Scalar other); // aten::eq.Scalar_out(Tensor self, Scalar other, *, Tensor(a!) out) -> Tensor(a!)
Tensor & eq_out(Tensor & out, const Tensor & self, const Tensor & other); // aten::eq.Tensor_out(Tensor self, Tensor other, *, Tensor(a!) out) -> Tensor(a!)
bool equal(const Tensor & self, const Tensor & other); // aten::equal(Tensor self, Tensor other) -> bool
Tensor erf(const Tensor & self); // aten::erf(Tensor self) -> Tensor
Tensor & erf_(Tensor & self); // aten::erf_(Tensor(a!) self) -> Tensor(a!)
Tensor & erf_out(Tensor & out, const Tensor & self); // aten::erf.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)
Tensor erfc(const Tensor & self); // aten::erfc(Tensor self) -> Tensor
Tensor & erfc_(Tensor & self); // aten::erfc_(Tensor(a!) self) -> Tensor(a!)
Tensor & erfc_out(Tensor & out, const Tensor & self); // aten::erfc.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)
Tensor erfinv(const Tensor & self); // aten::erfinv(Tensor self) -> Tensor
Tensor & erfinv_(Tensor & self); // aten::erfinv_(Tensor(a!) self) -> Tensor(a!)
Tensor & erfinv_out(Tensor & out, const Tensor & self); // aten::erfinv.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)
Tensor exp(const Tensor & self); // aten::exp(Tensor self) -> Tensor
Tensor & exp_(Tensor & self); // aten::exp_(Tensor(a!) self) -> Tensor(a!)
Tensor & exp_out(Tensor & out, const Tensor & self); // aten::exp.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)
Tensor expand(const Tensor & self, IntArrayRef size, bool implicit); // aten::expand(Tensor(a) self, int[] size, *, bool implicit=False) -> Tensor(a)
Tensor expm1(const Tensor & self); // aten::expm1(Tensor self) -> Tensor
Tensor & expm1_(Tensor & self); // aten::expm1_(Tensor(a!) self) -> Tensor(a!)
Tensor & expm1_out(Tensor & out, const Tensor & self); // aten::expm1.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)
Tensor & exponential_(Tensor & self, double lambd, Generator * generator); // aten::exponential_(Tensor(a!) self, float lambd=1, *, Generator? generator=None) -> Tensor(a!)
Tensor & eye_out(Tensor & out, int64_t n); // aten::eye.out(int n, *, Tensor(a!) out) -> Tensor(a!)
Tensor & eye_out(Tensor & out, int64_t n, int64_t m); // aten::eye.m_out(int n, int m, *, Tensor(a!) out) -> Tensor(a!)
Tensor fake_quantize_per_channel_affine(const Tensor & self, const Tensor & scale, const Tensor & zero_point, int64_t axis, int64_t quant_min, int64_t quant_max); // aten::fake_quantize_per_channel_affine(Tensor self, Tensor scale, Tensor zero_point, int axis, int quant_min, int quant_max) -> Tensor
Tensor fake_quantize_per_channel_affine_backward(const Tensor & grad, const Tensor & self, const Tensor & scale, const Tensor & zero_point, int64_t axis, int64_t quant_min, int64_t quant_max); // aten::fake_quantize_per_channel_affine_backward(Tensor grad, Tensor self, Tensor scale, Tensor zero_point, int axis, int quant_min, int quant_max) -> Tensor
Tensor fake_quantize_per_tensor_affine(const Tensor & self, double scale, int64_t zero_point, int64_t quant_min, int64_t quant_max); // aten::fake_quantize_per_tensor_affine(Tensor self, float scale, int zero_point, int quant_min, int quant_max) -> Tensor
Tensor fake_quantize_per_tensor_affine_backward(const Tensor & grad, const Tensor & self, double scale, int64_t zero_point, int64_t quant_min, int64_t quant_max); // aten::fake_quantize_per_tensor_affine_backward(Tensor grad, Tensor self, float scale, int zero_point, int quant_min, int quant_max) -> Tensor
Tensor & fill_(Tensor & self, Scalar value); // aten::fill_.Scalar(Tensor(a!) self, Scalar value) -> Tensor(a!)
Tensor & fill_(Tensor & self, const Tensor & value); // aten::fill_.Tensor(Tensor(a!) self, Tensor value) -> Tensor(a!)
Tensor flip(const Tensor & self, IntArrayRef dims); // aten::flip(Tensor self, int[] dims) -> Tensor
Tensor floor(const Tensor & self); // aten::floor(Tensor self) -> Tensor
Tensor & floor_(Tensor & self); // aten::floor_(Tensor(a!) self) -> Tensor(a!)
Tensor & floor_out(Tensor & out, const Tensor & self); // aten::floor.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)
Tensor fmod(const Tensor & self, Scalar other); // aten::fmod.Scalar(Tensor self, Scalar other) -> Tensor
Tensor fmod(const Tensor & self, const Tensor & other); // aten::fmod.Tensor(Tensor self, Tensor other) -> Tensor
Tensor & fmod_(Tensor & self, Scalar other); // aten::fmod_.Scalar(Tensor(a!) self, Scalar other) -> Tensor(a!)
Tensor & fmod_(Tensor & self, const Tensor & other); // aten::fmod_.Tensor(Tensor(a!) self, Tensor other) -> Tensor(a!)
Tensor & fmod_out(Tensor & out, const Tensor & self, Scalar other); // aten::fmod.Scalar_out(Tensor self, Scalar other, *, Tensor(a!) out) -> Tensor(a!)
Tensor & fmod_out(Tensor & out, const Tensor & self, const Tensor & other); // aten::fmod.Tensor_out(Tensor self, Tensor other, *, Tensor(a!) out) -> Tensor(a!)
Tensor frac(const Tensor & self); // aten::frac(Tensor self) -> Tensor
Tensor & frac_(Tensor & self); // aten::frac_(Tensor(a!) self) -> Tensor(a!)
Tensor & frac_out(Tensor & out, const Tensor & self); // aten::frac.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)
std::tuple<Tensor,Tensor> fractional_max_pool2d(const Tensor & self, IntArrayRef kernel_size, IntArrayRef output_size, const Tensor & random_samples); // aten::fractional_max_pool2d(Tensor self, int[2] kernel_size, int[2] output_size, Tensor random_samples) -> (Tensor, Tensor)
Tensor fractional_max_pool2d_backward(const Tensor & grad_output, const Tensor & self, IntArrayRef kernel_size, IntArrayRef output_size, const Tensor & indices); // aten::fractional_max_pool2d_backward(Tensor grad_output, Tensor self, int[2] kernel_size, int[2] output_size, Tensor indices) -> Tensor
Tensor & fractional_max_pool2d_backward_out(Tensor & grad_input, const Tensor & grad_output, const Tensor & self, IntArrayRef kernel_size, IntArrayRef output_size, const Tensor & indices); // aten::fractional_max_pool2d_backward.grad_input(Tensor grad_output, Tensor self, int[2] kernel_size, int[2] output_size, Tensor indices, *, Tensor(a!) grad_input) -> Tensor(a!)
std::tuple<Tensor &,Tensor &> fractional_max_pool2d_out(Tensor & output, Tensor & indices, const Tensor & self, IntArrayRef kernel_size, IntArrayRef output_size, const Tensor & random_samples); // aten::fractional_max_pool2d.output(Tensor self, int[2] kernel_size, int[2] output_size, Tensor random_samples, *, Tensor(a!) output, Tensor(b!) indices) -> (Tensor(a!), Tensor(b!))
std::tuple<Tensor,Tensor> fractional_max_pool3d(const Tensor & self, IntArrayRef kernel_size, IntArrayRef output_size, const Tensor & random_samples); // aten::fractional_max_pool3d(Tensor self, int[3] kernel_size, int[3] output_size, Tensor random_samples) -> (Tensor, Tensor)
Tensor fractional_max_pool3d_backward(const Tensor & grad_output, const Tensor & self, IntArrayRef kernel_size, IntArrayRef output_size, const Tensor & indices); // aten::fractional_max_pool3d_backward(Tensor grad_output, Tensor self, int[3] kernel_size, int[3] output_size, Tensor indices) -> Tensor
Tensor & fractional_max_pool3d_backward_out(Tensor & grad_input, const Tensor & grad_output, const Tensor & self, IntArrayRef kernel_size, IntArrayRef output_size, const Tensor & indices); // aten::fractional_max_pool3d_backward.grad_input(Tensor grad_output, Tensor self, int[3] kernel_size, int[3] output_size, Tensor indices, *, Tensor(a!) grad_input) -> Tensor(a!)
std::tuple<Tensor &,Tensor &> fractional_max_pool3d_out(Tensor & output, Tensor & indices, const Tensor & self, IntArrayRef kernel_size, IntArrayRef output_size, const Tensor & random_samples); // aten::fractional_max_pool3d.output(Tensor self, int[3] kernel_size, int[3] output_size, Tensor random_samples, *, Tensor(a!) output, Tensor(b!) indices) -> (Tensor(a!), Tensor(b!))
Tensor from_file(std::string filename, c10::optional<bool> shared, c10::optional<int64_t> size, const TensorOptions & options); // aten::from_file(str filename, bool? shared=None, int? size=0, *, ScalarType? dtype=None, Layout? layout=None, Device? device=None, bool? pin_memory=None) -> Tensor
Tensor gather(const Tensor & self, int64_t dim, const Tensor & index, bool sparse_grad); // aten::gather(Tensor self, int dim, Tensor index, *, bool sparse_grad=False) -> Tensor
Tensor & gather_out(Tensor & out, const Tensor & self, int64_t dim, const Tensor & index, bool sparse_grad); // aten::gather.out(Tensor self, int dim, Tensor index, *, bool sparse_grad=False, Tensor(a!) out) -> Tensor(a!)
Tensor ge(const Tensor & self, Scalar other); // aten::ge.Scalar(Tensor self, Scalar other) -> Tensor
Tensor ge(const Tensor & self, const Tensor & other); // aten::ge.Tensor(Tensor self, Tensor other) -> Tensor
Tensor & ge_(Tensor & self, Scalar other); // aten::ge_.Scalar(Tensor(a!) self, Scalar other) -> Tensor(a!)
Tensor & ge_(Tensor & self, const Tensor & other); // aten::ge_.Tensor(Tensor(a!) self, Tensor other) -> Tensor(a!)
Tensor & ge_out(Tensor & out, const Tensor & self, Scalar other); // aten::ge.Scalar_out(Tensor self, Scalar other, *, Tensor(a!) out) -> Tensor(a!)
Tensor & ge_out(Tensor & out, const Tensor & self, const Tensor & other); // aten::ge.Tensor_out(Tensor self, Tensor other, *, Tensor(a!) out) -> Tensor(a!)
Tensor gelu(const Tensor & self); // aten::gelu(Tensor self) -> Tensor
Tensor gelu_backward(const Tensor & grad, const Tensor & self); // aten::gelu_backward(Tensor grad, Tensor self) -> Tensor
Tensor & geometric_(Tensor & self, double p, Generator * generator); // aten::geometric_(Tensor(a!) self, float p, *, Generator? generator=None) -> Tensor(a!)
std::tuple<Tensor,Tensor> geqrf(const Tensor & self); // aten::geqrf(Tensor self) -> (Tensor a, Tensor tau)
std::tuple<Tensor &,Tensor &> geqrf_out(Tensor & a, Tensor & tau, const Tensor & self); // aten::geqrf.a(Tensor self, *, Tensor(a!) a, Tensor(b!) tau) -> (Tensor(a!) a, Tensor(b!) tau)
Tensor ger(const Tensor & self, const Tensor & vec2); // aten::ger(Tensor self, Tensor vec2) -> Tensor
Tensor & ger_out(Tensor & out, const Tensor & self, const Tensor & vec2); // aten::ger.out(Tensor self, Tensor vec2, *, Tensor(a!) out) -> Tensor(a!)
Tensor glu(const Tensor & self, int64_t dim); // aten::glu(Tensor self, int dim=-1) -> Tensor
Tensor glu_backward(const Tensor & grad_output, const Tensor & self, int64_t dim); // aten::glu_backward(Tensor grad_output, Tensor self, int dim) -> Tensor
Tensor & glu_backward_out(Tensor & grad_input, const Tensor & grad_output, const Tensor & self, int64_t dim); // aten::glu_backward.grad_input(Tensor grad_output, Tensor self, int dim, *, Tensor(a!) grad_input) -> Tensor(a!)
Tensor & glu_out(Tensor & out, const Tensor & self, int64_t dim); // aten::glu.out(Tensor self, int dim=-1, *, Tensor(a!) out) -> Tensor(a!)
Tensor grid_sampler_2d(const Tensor & input, const Tensor & grid, int64_t interpolation_mode, int64_t padding_mode, bool align_corners); // aten::grid_sampler_2d(Tensor input, Tensor grid, int interpolation_mode, int padding_mode, bool align_corners) -> Tensor
std::tuple<Tensor,Tensor> grid_sampler_2d_backward(const Tensor & grad_output, const Tensor & input, const Tensor & grid, int64_t interpolation_mode, int64_t padding_mode, bool align_corners); // aten::grid_sampler_2d_backward(Tensor grad_output, Tensor input, Tensor grid, int interpolation_mode, int padding_mode, bool align_corners) -> (Tensor, Tensor)
Tensor grid_sampler_3d(const Tensor & input, const Tensor & grid, int64_t interpolation_mode, int64_t padding_mode, bool align_corners); // aten::grid_sampler_3d(Tensor input, Tensor grid, int interpolation_mode, int padding_mode, bool align_corners) -> Tensor
std::tuple<Tensor,Tensor> grid_sampler_3d_backward(const Tensor & grad_output, const Tensor & input, const Tensor & grid, int64_t interpolation_mode, int64_t padding_mode, bool align_corners); // aten::grid_sampler_3d_backward(Tensor grad_output, Tensor input, Tensor grid, int interpolation_mode, int padding_mode, bool align_corners) -> (Tensor, Tensor)
Tensor gt(const Tensor & self, Scalar other); // aten::gt.Scalar(Tensor self, Scalar other) -> Tensor
Tensor gt(const Tensor & self, const Tensor & other); // aten::gt.Tensor(Tensor self, Tensor other) -> Tensor
Tensor & gt_(Tensor & self, Scalar other); // aten::gt_.Scalar(Tensor(a!) self, Scalar other) -> Tensor(a!)
Tensor & gt_(Tensor & self, const Tensor & other); // aten::gt_.Tensor(Tensor(a!) self, Tensor other) -> Tensor(a!)
Tensor & gt_out(Tensor & out, const Tensor & self, Scalar other); // aten::gt.Scalar_out(Tensor self, Scalar other, *, Tensor(a!) out) -> Tensor(a!)
Tensor & gt_out(Tensor & out, const Tensor & self, const Tensor & other); // aten::gt.Tensor_out(Tensor self, Tensor other, *, Tensor(a!) out) -> Tensor(a!)
Tensor hardshrink(const Tensor & self, Scalar lambd); // aten::hardshrink(Tensor self, Scalar lambd=0.5) -> Tensor
Tensor hardshrink_backward(const Tensor & grad_out, const Tensor & self, Scalar lambd); // aten::hardshrink_backward(Tensor grad_out, Tensor self, Scalar lambd) -> Tensor
Tensor hardtanh(const Tensor & self, Scalar min_val, Scalar max_val); // aten::hardtanh(Tensor self, Scalar min_val=-1, Scalar max_val=1) -> Tensor
Tensor & hardtanh_(Tensor & self, Scalar min_val, Scalar max_val); // aten::hardtanh_(Tensor(a!) self, Scalar min_val=-1, Scalar max_val=1) -> Tensor(a!)
Tensor hardtanh_backward(const Tensor & grad_output, const Tensor & self, Scalar min_val, Scalar max_val); // aten::hardtanh_backward(Tensor grad_output, Tensor self, Scalar min_val, Scalar max_val) -> Tensor
Tensor & hardtanh_backward_out(Tensor & grad_input, const Tensor & grad_output, const Tensor & self, Scalar min_val, Scalar max_val); // aten::hardtanh_backward.grad_input(Tensor grad_output, Tensor self, Scalar min_val, Scalar max_val, *, Tensor(a!) grad_input) -> Tensor(a!)
Tensor & hardtanh_out(Tensor & out, const Tensor & self, Scalar min_val, Scalar max_val); // aten::hardtanh.out(Tensor self, Scalar min_val=-1, Scalar max_val=1, *, Tensor(a!) out) -> Tensor(a!)
Tensor histc(const Tensor & self, int64_t bins, Scalar min, Scalar max); // aten::histc(Tensor self, int bins=100, Scalar min=0, Scalar max=0) -> Tensor
Tensor & histc_out(Tensor & out, const Tensor & self, int64_t bins, Scalar min, Scalar max); // aten::histc.out(Tensor self, int bins=100, Scalar min=0, Scalar max=0, *, Tensor(a!) out) -> Tensor(a!)
Tensor hspmm(const Tensor & mat1, const Tensor & mat2); // aten::hspmm(Tensor mat1, Tensor mat2) -> Tensor
Tensor & hspmm_out(Tensor & out, const Tensor & mat1, const Tensor & mat2); // aten::hspmm.out(Tensor mat1, Tensor mat2, *, Tensor(a!) out) -> Tensor(a!)
Tensor im2col(const Tensor & self, IntArrayRef kernel_size, IntArrayRef dilation, IntArrayRef padding, IntArrayRef stride); // aten::im2col(Tensor self, int[2] kernel_size, int[2] dilation, int[2] padding, int[2] stride) -> Tensor
Tensor im2col_backward(const Tensor & grad_output, IntArrayRef input_size, IntArrayRef kernel_size, IntArrayRef dilation, IntArrayRef padding, IntArrayRef stride); // aten::im2col_backward(Tensor grad_output, int[2] input_size, int[2] kernel_size, int[2] dilation, int[2] padding, int[2] stride) -> Tensor
Tensor & im2col_backward_out(Tensor & grad_input, const Tensor & grad_output, IntArrayRef input_size, IntArrayRef kernel_size, IntArrayRef dilation, IntArrayRef padding, IntArrayRef stride); // aten::im2col_backward.grad_input(Tensor grad_output, int[2] input_size, int[2] kernel_size, int[2] dilation, int[2] padding, int[2] stride, *, Tensor(a!) grad_input) -> Tensor(a!)
Tensor & im2col_out(Tensor & out, const Tensor & self, IntArrayRef kernel_size, IntArrayRef dilation, IntArrayRef padding, IntArrayRef stride); // aten::im2col.out(Tensor self, int[2] kernel_size, int[2] dilation, int[2] padding, int[2] stride, *, Tensor(a!) out) -> Tensor(a!)
Tensor imag(const Tensor & self); // aten::imag(Tensor self) -> Tensor
Tensor & imag_out(Tensor & out, const Tensor & self); // aten::imag.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)
Tensor index(const Tensor & self, TensorList indices); // aten::index.Tensor(Tensor self, Tensor?[] indices) -> Tensor
Tensor & index_add_(Tensor & self, int64_t dim, const Tensor & index, const Tensor & source); // aten::index_add_(Tensor(a!) self, int dim, Tensor index, Tensor source) -> Tensor(a!)
Tensor & index_copy_(Tensor & self, int64_t dim, const Tensor & index, const Tensor & source); // aten::index_copy_(Tensor(a!) self, int dim, Tensor index, Tensor source) -> Tensor(a!)
Tensor & index_fill_(Tensor & self, int64_t dim, const Tensor & index, Scalar value); // aten::index_fill_.int_Scalar(Tensor(a!) self, int dim, Tensor index, Scalar value) -> Tensor(a!)
Tensor & index_fill_(Tensor & self, int64_t dim, const Tensor & index, const Tensor & value); // aten::index_fill_.int_Tensor(Tensor(a!) self, int dim, Tensor index, Tensor value) -> Tensor(a!)
Tensor & index_put_(Tensor & self, TensorList indices, const Tensor & values, bool accumulate); // aten::index_put_(Tensor(a!) self, Tensor?[] indices, Tensor values, bool accumulate=False) -> Tensor(a!)
Tensor index_select(const Tensor & self, int64_t dim, const Tensor & index); // aten::index_select(Tensor self, int dim, Tensor index) -> Tensor
Tensor & index_select_out(Tensor & out, const Tensor & self, int64_t dim, const Tensor & index); // aten::index_select.out(Tensor self, int dim, Tensor index, *, Tensor(a!) out) -> Tensor(a!)
Tensor indices(const Tensor & self); // aten::indices(Tensor(a) self) -> Tensor(a)
Tensor int_repr(const Tensor & self); // aten::int_repr(Tensor self) -> Tensor
Tensor inverse(const Tensor & self); // aten::inverse(Tensor self) -> Tensor
Tensor & inverse_out(Tensor & out, const Tensor & self); // aten::inverse.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)
bool is_coalesced(const Tensor & self); // aten::is_coalesced(Tensor self) -> bool
bool is_set_to(const Tensor & self, const Tensor & tensor); // aten::is_set_to(Tensor self, Tensor tensor) -> bool
Tensor kl_div(const Tensor & self, const Tensor & target, int64_t reduction); // aten::kl_div(Tensor self, Tensor target, int reduction=Mean) -> Tensor
Tensor kl_div_backward(const Tensor & grad_output, const Tensor & self, const Tensor & target, int64_t reduction); // aten::kl_div_backward(Tensor grad_output, Tensor self, Tensor target, int reduction=Mean) -> Tensor
std::tuple<Tensor,Tensor> kthvalue(const Tensor & self, int64_t k, int64_t dim, bool keepdim); // aten::kthvalue(Tensor self, int k, int dim=-1, bool keepdim=False) -> (Tensor values, Tensor indices)
std::tuple<Tensor &,Tensor &> kthvalue_out(Tensor & values, Tensor & indices, const Tensor & self, int64_t k, int64_t dim, bool keepdim); // aten::kthvalue.values(Tensor self, int k, int dim=-1, bool keepdim=False, *, Tensor(a!) values, Tensor(b!) indices) -> (Tensor(a!) values, Tensor(b!) indices)
Tensor l1_loss(const Tensor & self, const Tensor & target, int64_t reduction); // aten::l1_loss(Tensor self, Tensor target, int reduction=Mean) -> Tensor
Tensor l1_loss_backward(const Tensor & grad_output, const Tensor & self, const Tensor & target, int64_t reduction); // aten::l1_loss_backward(Tensor grad_output, Tensor self, Tensor target, int reduction) -> Tensor
Tensor & l1_loss_backward_out(Tensor & grad_input, const Tensor & grad_output, const Tensor & self, const Tensor & target, int64_t reduction); // aten::l1_loss_backward.grad_input(Tensor grad_output, Tensor self, Tensor target, int reduction, *, Tensor(a!) grad_input) -> Tensor(a!)
Tensor & l1_loss_out(Tensor & out, const Tensor & self, const Tensor & target, int64_t reduction); // aten::l1_loss.out(Tensor self, Tensor target, int reduction=Mean, *, Tensor(a!) out) -> Tensor(a!)
Tensor le(const Tensor & self, Scalar other); // aten::le.Scalar(Tensor self, Scalar other) -> Tensor
Tensor le(const Tensor & self, const Tensor & other); // aten::le.Tensor(Tensor self, Tensor other) -> Tensor
Tensor & le_(Tensor & self, Scalar other); // aten::le_.Scalar(Tensor(a!) self, Scalar other) -> Tensor(a!)
Tensor & le_(Tensor & self, const Tensor & other); // aten::le_.Tensor(Tensor(a!) self, Tensor other) -> Tensor(a!)
Tensor & le_out(Tensor & out, const Tensor & self, Scalar other); // aten::le.Scalar_out(Tensor self, Scalar other, *, Tensor(a!) out) -> Tensor(a!)
Tensor & le_out(Tensor & out, const Tensor & self, const Tensor & other); // aten::le.Tensor_out(Tensor self, Tensor other, *, Tensor(a!) out) -> Tensor(a!)
Tensor leaky_relu(const Tensor & self, Scalar negative_slope); // aten::leaky_relu(Tensor self, Scalar negative_slope=0.01) -> Tensor
Tensor & leaky_relu_(Tensor & self, Scalar negative_slope); // aten::leaky_relu_(Tensor(a!) self, Scalar negative_slope=0.01) -> Tensor(a!)
Tensor leaky_relu_backward(const Tensor & grad_output, const Tensor & self, Scalar negative_slope); // aten::leaky_relu_backward(Tensor grad_output, Tensor self, Scalar negative_slope) -> Tensor
Tensor & leaky_relu_backward_out(Tensor & grad_input, const Tensor & grad_output, const Tensor & self, Scalar negative_slope); // aten::leaky_relu_backward.grad_input(Tensor grad_output, Tensor self, Scalar negative_slope, *, Tensor(a!) grad_input) -> Tensor(a!)
Tensor & leaky_relu_out(Tensor & out, const Tensor & self, Scalar negative_slope); // aten::leaky_relu.out(Tensor self, Scalar negative_slope=0.01, *, Tensor(a!) out) -> Tensor(a!)
Tensor lerp(const Tensor & self, const Tensor & end, Scalar weight); // aten::lerp.Scalar(Tensor self, Tensor end, Scalar weight) -> Tensor
Tensor lerp(const Tensor & self, const Tensor & end, const Tensor & weight); // aten::lerp.Tensor(Tensor self, Tensor end, Tensor weight) -> Tensor
Tensor & lerp_(Tensor & self, const Tensor & end, Scalar weight); // aten::lerp_.Scalar(Tensor(a!) self, Tensor end, Scalar weight) -> Tensor(a!)
Tensor & lerp_(Tensor & self, const Tensor & end, const Tensor & weight); // aten::lerp_.Tensor(Tensor(a!) self, Tensor end, Tensor weight) -> Tensor(a!)
Tensor & lerp_out(Tensor & out, const Tensor & self, const Tensor & end, Scalar weight); // aten::lerp.Scalar_out(Tensor self, Tensor end, Scalar weight, *, Tensor(a!) out) -> Tensor(a!)
Tensor & lerp_out(Tensor & out, const Tensor & self, const Tensor & end, const Tensor & weight); // aten::lerp.Tensor_out(Tensor self, Tensor end, Tensor weight, *, Tensor(a!) out) -> Tensor(a!)
Tensor lgamma(const Tensor & self); // aten::lgamma(Tensor self) -> Tensor
Tensor & lgamma_(Tensor & self); // aten::lgamma_(Tensor(a!) self) -> Tensor(a!)
Tensor & lgamma_out(Tensor & out, const Tensor & self); // aten::lgamma.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)
Tensor & linspace_out(Tensor & out, Scalar start, Scalar end, int64_t steps); // aten::linspace.out(Scalar start, Scalar end, int steps=100, *, Tensor(a!) out) -> Tensor(a!)
Tensor log(const Tensor & self); // aten::log(Tensor self) -> Tensor
Tensor log10(const Tensor & self); // aten::log10(Tensor self) -> Tensor
Tensor & log10_(Tensor & self); // aten::log10_(Tensor(a!) self) -> Tensor(a!)
Tensor & log10_out(Tensor & out, const Tensor & self); // aten::log10.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)
Tensor log1p(const Tensor & self); // aten::log1p(Tensor self) -> Tensor
Tensor & log1p_(Tensor & self); // aten::log1p_(Tensor(a!) self) -> Tensor(a!)
Tensor & log1p_out(Tensor & out, const Tensor & self); // aten::log1p.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)
Tensor log2(const Tensor & self); // aten::log2(Tensor self) -> Tensor
Tensor & log2_(Tensor & self); // aten::log2_(Tensor(a!) self) -> Tensor(a!)
Tensor & log2_out(Tensor & out, const Tensor & self); // aten::log2.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)
Tensor & log_(Tensor & self); // aten::log_(Tensor(a!) self) -> Tensor(a!)
Tensor & log_normal_(Tensor & self, double mean, double std, Generator * generator); // aten::log_normal_(Tensor(a!) self, float mean=1, float std=2, *, Generator? generator=None) -> Tensor(a!)
Tensor & log_out(Tensor & out, const Tensor & self); // aten::log.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)
Tensor log_sigmoid_backward(const Tensor & grad_output, const Tensor & self, const Tensor & buffer); // aten::log_sigmoid_backward(Tensor grad_output, Tensor self, Tensor buffer) -> Tensor
Tensor & log_sigmoid_backward_out(Tensor & grad_input, const Tensor & grad_output, const Tensor & self, const Tensor & buffer); // aten::log_sigmoid_backward.grad_input(Tensor grad_output, Tensor self, Tensor buffer, *, Tensor(a!) grad_input) -> Tensor(a!)
std::tuple<Tensor,Tensor> log_sigmoid_forward(const Tensor & self); // aten::log_sigmoid_forward(Tensor self) -> (Tensor output, Tensor buffer)
std::tuple<Tensor &,Tensor &> log_sigmoid_forward_out(Tensor & output, Tensor & buffer, const Tensor & self); // aten::log_sigmoid_forward.output(Tensor self, *, Tensor(a!) output, Tensor(b!) buffer) -> (Tensor(a!), Tensor(b!))
Tensor logdet(const Tensor & self); // aten::logdet(Tensor self) -> Tensor
Tensor & logical_not_out(Tensor & out, const Tensor & self); // aten::logical_not.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)
Tensor & logical_xor_out(Tensor & out, const Tensor & self, const Tensor & other); // aten::logical_xor.out(Tensor self, Tensor other, *, Tensor(a!) out) -> Tensor(a!)
Tensor & logspace_out(Tensor & out, Scalar start, Scalar end, int64_t steps, double base); // aten::logspace.out(Scalar start, Scalar end, int steps=100, float base=10.0, *, Tensor(a!) out) -> Tensor(a!)
Tensor logsumexp(const Tensor & self, IntArrayRef dim, bool keepdim); // aten::logsumexp(Tensor self, int[1] dim, bool keepdim=False) -> Tensor
Tensor & logsumexp_out(Tensor & out, const Tensor & self, IntArrayRef dim, bool keepdim); // aten::logsumexp.out(Tensor self, int[1] dim, bool keepdim=False, *, Tensor(a!) out) -> Tensor(a!)
std::tuple<Tensor,Tensor> lstsq(const Tensor & self, const Tensor & A); // aten::lstsq(Tensor self, Tensor A) -> (Tensor solution, Tensor QR)
std::tuple<Tensor &,Tensor &> lstsq_out(Tensor & X, Tensor & qr, const Tensor & self, const Tensor & A); // aten::lstsq.X(Tensor self, Tensor A, *, Tensor(a!) X, Tensor(b!) qr) -> (Tensor(a!) solution, Tensor(b!) QR)
Tensor lt(const Tensor & self, Scalar other); // aten::lt.Scalar(Tensor self, Scalar other) -> Tensor
Tensor lt(const Tensor & self, const Tensor & other); // aten::lt.Tensor(Tensor self, Tensor other) -> Tensor
Tensor & lt_(Tensor & self, Scalar other); // aten::lt_.Scalar(Tensor(a!) self, Scalar other) -> Tensor(a!)
Tensor & lt_(Tensor & self, const Tensor & other); // aten::lt_.Tensor(Tensor(a!) self, Tensor other) -> Tensor(a!)
Tensor & lt_out(Tensor & out, const Tensor & self, Scalar other); // aten::lt.Scalar_out(Tensor self, Scalar other, *, Tensor(a!) out) -> Tensor(a!)
Tensor & lt_out(Tensor & out, const Tensor & self, const Tensor & other); // aten::lt.Tensor_out(Tensor self, Tensor other, *, Tensor(a!) out) -> Tensor(a!)
Tensor lu_solve(const Tensor & self, const Tensor & LU_data, const Tensor & LU_pivots); // aten::lu_solve(Tensor self, Tensor LU_data, Tensor LU_pivots) -> Tensor
Tensor & lu_solve_out(Tensor & out, const Tensor & self, const Tensor & LU_data, const Tensor & LU_pivots); // aten::lu_solve.out(Tensor self, Tensor LU_data, Tensor LU_pivots, *, Tensor(a!) out) -> Tensor(a!)
Tensor & masked_fill_(Tensor & self, const Tensor & mask, Scalar value); // aten::masked_fill_.Scalar(Tensor(a!) self, Tensor mask, Scalar value) -> Tensor(a!)
Tensor & masked_fill_(Tensor & self, const Tensor & mask, const Tensor & value); // aten::masked_fill_.Tensor(Tensor(a!) self, Tensor mask, Tensor value) -> Tensor(a!)
Tensor & masked_scatter_(Tensor & self, const Tensor & mask, const Tensor & source); // aten::masked_scatter_(Tensor(a!) self, Tensor mask, Tensor source) -> Tensor(a!)
Tensor masked_select(const Tensor & self, const Tensor & mask); // aten::masked_select(Tensor self, Tensor mask) -> Tensor
Tensor & masked_select_out(Tensor & out, const Tensor & self, const Tensor & mask); // aten::masked_select.out(Tensor self, Tensor mask, *, Tensor(a!) out) -> Tensor(a!)
std::tuple<Tensor,Tensor> max(const Tensor & self, int64_t dim, bool keepdim); // aten::max.dim(Tensor self, int dim, bool keepdim=False) -> (Tensor values, Tensor indices)
Tensor max(const Tensor & self, const Tensor & other); // aten::max.other(Tensor self, Tensor other) -> Tensor
Tensor max(const Tensor & self); // aten::max(Tensor self) -> Tensor
std::tuple<Tensor &,Tensor &> max_out(Tensor & max, Tensor & max_values, const Tensor & self, int64_t dim, bool keepdim); // aten::max.dim_max(Tensor self, int dim, bool keepdim=False, *, Tensor(a!) max, Tensor(b!) max_values) -> (Tensor(a!) values, Tensor(b!) indices)
Tensor & max_out(Tensor & out, const Tensor & self, const Tensor & other); // aten::max.out(Tensor self, Tensor other, *, Tensor(a!) out) -> Tensor(a!)
std::tuple<Tensor,Tensor> max_pool2d_with_indices(const Tensor & self, IntArrayRef kernel_size, IntArrayRef stride, IntArrayRef padding, IntArrayRef dilation, bool ceil_mode); // aten::max_pool2d_with_indices(Tensor self, int[2] kernel_size, int[2] stride=[], int[2] padding=0, int[2] dilation=1, bool ceil_mode=False) -> (Tensor, Tensor)
Tensor max_pool2d_with_indices_backward(const Tensor & grad_output, const Tensor & self, IntArrayRef kernel_size, IntArrayRef stride, IntArrayRef padding, IntArrayRef dilation, bool ceil_mode, const Tensor & indices); // aten::max_pool2d_with_indices_backward(Tensor grad_output, Tensor self, int[2] kernel_size, int[2] stride, int[2] padding, int[2] dilation, bool ceil_mode, Tensor indices) -> Tensor
Tensor & max_pool2d_with_indices_backward_out(Tensor & grad_input, const Tensor & grad_output, const Tensor & self, IntArrayRef kernel_size, IntArrayRef stride, IntArrayRef padding, IntArrayRef dilation, bool ceil_mode, const Tensor & indices); // aten::max_pool2d_with_indices_backward.grad_input(Tensor grad_output, Tensor self, int[2] kernel_size, int[2] stride, int[2] padding, int[2] dilation, bool ceil_mode, Tensor indices, *, Tensor(a!) grad_input) -> Tensor(a!)
std::tuple<Tensor &,Tensor &> max_pool2d_with_indices_out(Tensor & out, Tensor & indices, const Tensor & self, IntArrayRef kernel_size, IntArrayRef stride, IntArrayRef padding, IntArrayRef dilation, bool ceil_mode); // aten::max_pool2d_with_indices.out(Tensor self, int[2] kernel_size, int[2] stride=[], int[2] padding=0, int[2] dilation=1, bool ceil_mode=False, *, Tensor(a!) out, Tensor(b!) indices) -> (Tensor(a!), Tensor(b!))
std::tuple<Tensor,Tensor> max_pool3d_with_indices(const Tensor & self, IntArrayRef kernel_size, IntArrayRef stride, IntArrayRef padding, IntArrayRef dilation, bool ceil_mode); // aten::max_pool3d_with_indices(Tensor self, int[3] kernel_size, int[3] stride=[], int[3] padding=0, int[3] dilation=1, bool ceil_mode=False) -> (Tensor, Tensor)
Tensor max_pool3d_with_indices_backward(const Tensor & grad_output, const Tensor & self, IntArrayRef kernel_size, IntArrayRef stride, IntArrayRef padding, IntArrayRef dilation, bool ceil_mode, const Tensor & indices); // aten::max_pool3d_with_indices_backward(Tensor grad_output, Tensor self, int[3] kernel_size, int[3] stride, int[3] padding, int[3] dilation, bool ceil_mode, Tensor indices) -> Tensor
Tensor & max_pool3d_with_indices_backward_out(Tensor & grad_input, const Tensor & grad_output, const Tensor & self, IntArrayRef kernel_size, IntArrayRef stride, IntArrayRef padding, IntArrayRef dilation, bool ceil_mode, const Tensor & indices); // aten::max_pool3d_with_indices_backward.grad_input(Tensor grad_output, Tensor self, int[3] kernel_size, int[3] stride, int[3] padding, int[3] dilation, bool ceil_mode, Tensor indices, *, Tensor(a!) grad_input) -> Tensor(a!)
std::tuple<Tensor &,Tensor &> max_pool3d_with_indices_out(Tensor & out, Tensor & indices, const Tensor & self, IntArrayRef kernel_size, IntArrayRef stride, IntArrayRef padding, IntArrayRef dilation, bool ceil_mode); // aten::max_pool3d_with_indices.out(Tensor self, int[3] kernel_size, int[3] stride=[], int[3] padding=0, int[3] dilation=1, bool ceil_mode=False, *, Tensor(a!) out, Tensor(b!) indices) -> (Tensor(a!), Tensor(b!))
Tensor max_unpool2d(const Tensor & self, const Tensor & indices, IntArrayRef output_size); // aten::max_unpool2d(Tensor self, Tensor indices, int[2] output_size) -> Tensor
Tensor max_unpool2d_backward(const Tensor & grad_output, const Tensor & self, const Tensor & indices, IntArrayRef output_size); // aten::max_unpool2d_backward(Tensor grad_output, Tensor self, Tensor indices, int[2] output_size) -> Tensor
Tensor & max_unpool2d_backward_out(Tensor & grad_input, const Tensor & grad_output, const Tensor & self, const Tensor & indices, IntArrayRef output_size); // aten::max_unpool2d_backward.grad_input(Tensor grad_output, Tensor self, Tensor indices, int[2] output_size, *, Tensor(a!) grad_input) -> Tensor(a!)
Tensor & max_unpool2d_out(Tensor & out, const Tensor & self, const Tensor & indices, IntArrayRef output_size); // aten::max_unpool2d.out(Tensor self, Tensor indices, int[2] output_size, *, Tensor(a!) out) -> Tensor(a!)
Tensor max_unpool3d(const Tensor & self, const Tensor & indices, IntArrayRef output_size, IntArrayRef stride, IntArrayRef padding); // aten::max_unpool3d(Tensor self, Tensor indices, int[3] output_size, int[3] stride, int[3] padding) -> Tensor
Tensor max_unpool3d_backward(const Tensor & grad_output, const Tensor & self, const Tensor & indices, IntArrayRef output_size, IntArrayRef stride, IntArrayRef padding); // aten::max_unpool3d_backward(Tensor grad_output, Tensor self, Tensor indices, int[3] output_size, int[3] stride, int[3] padding) -> Tensor
Tensor & max_unpool3d_backward_out(Tensor & grad_input, const Tensor & grad_output, const Tensor & self, const Tensor & indices, IntArrayRef output_size, IntArrayRef stride, IntArrayRef padding); // aten::max_unpool3d_backward.grad_input(Tensor grad_output, Tensor self, Tensor indices, int[3] output_size, int[3] stride, int[3] padding, *, Tensor(a!) grad_input) -> Tensor(a!)
Tensor & max_unpool3d_out(Tensor & out, const Tensor & self, const Tensor & indices, IntArrayRef output_size, IntArrayRef stride, IntArrayRef padding); // aten::max_unpool3d.out(Tensor self, Tensor indices, int[3] output_size, int[3] stride, int[3] padding, *, Tensor(a!) out) -> Tensor(a!)
Tensor mean(const Tensor & self, c10::optional<ScalarType> dtype); // aten::mean(Tensor self, *, ScalarType? dtype=None) -> Tensor
Tensor mean(const Tensor & self, IntArrayRef dim, bool keepdim, c10::optional<ScalarType> dtype); // aten::mean.dim(Tensor self, int[1] dim, bool keepdim=False, *, ScalarType? dtype=None) -> Tensor
Tensor & mean_out(Tensor & out, const Tensor & self, IntArrayRef dim, bool keepdim, c10::optional<ScalarType> dtype); // aten::mean.out(Tensor self, int[1] dim, bool keepdim=False, *, ScalarType? dtype=None, Tensor(a!) out) -> Tensor(a!)
std::tuple<Tensor,Tensor> median(const Tensor & self, int64_t dim, bool keepdim); // aten::median.dim(Tensor self, int dim, bool keepdim=False) -> (Tensor values, Tensor indices)
Tensor median(const Tensor & self); // aten::median(Tensor self) -> Tensor
std::tuple<Tensor &,Tensor &> median_out(Tensor & values, Tensor & indices, const Tensor & self, int64_t dim, bool keepdim); // aten::median.dim_values(Tensor self, int dim, bool keepdim=False, *, Tensor(a!) values, Tensor(b!) indices) -> (Tensor(a!) values, Tensor(b!) indices)
std::tuple<Tensor,Tensor> min(const Tensor & self, int64_t dim, bool keepdim); // aten::min.dim(Tensor self, int dim, bool keepdim=False) -> (Tensor values, Tensor indices)
Tensor min(const Tensor & self, const Tensor & other); // aten::min.other(Tensor self, Tensor other) -> Tensor
Tensor min(const Tensor & self); // aten::min(Tensor self) -> Tensor
std::tuple<Tensor &,Tensor &> min_out(Tensor & min, Tensor & min_indices, const Tensor & self, int64_t dim, bool keepdim); // aten::min.dim_min(Tensor self, int dim, bool keepdim=False, *, Tensor(a!) min, Tensor(b!) min_indices) -> (Tensor(a!) values, Tensor(b!) indices)
Tensor & min_out(Tensor & out, const Tensor & self, const Tensor & other); // aten::min.out(Tensor self, Tensor other, *, Tensor(a!) out) -> Tensor(a!)
std::tuple<Tensor,Tensor,Tensor> miopen_batch_norm(const Tensor & input, const Tensor & weight, const Tensor & bias, const Tensor & running_mean, const Tensor & running_var, bool training, double exponential_average_factor, double epsilon); // aten::miopen_batch_norm(Tensor input, Tensor weight, Tensor? bias, Tensor? running_mean, Tensor? running_var, bool training, float exponential_average_factor, float epsilon) -> (Tensor, Tensor, Tensor)
std::tuple<Tensor,Tensor,Tensor> miopen_batch_norm_backward(const Tensor & input, const Tensor & grad_output, const Tensor & weight, const Tensor & running_mean, const Tensor & running_var, const Tensor & save_mean, const Tensor & save_var, double epsilon); // aten::miopen_batch_norm_backward(Tensor input, Tensor grad_output, Tensor weight, Tensor? running_mean, Tensor? running_var, Tensor? save_mean, Tensor? save_var, float epsilon) -> (Tensor, Tensor, Tensor)
Tensor miopen_convolution(const Tensor & self, const Tensor & weight, const Tensor & bias, IntArrayRef padding, IntArrayRef stride, IntArrayRef dilation, int64_t groups, bool benchmark, bool deterministic); // aten::miopen_convolution(Tensor self, Tensor weight, Tensor? bias, int[] padding, int[] stride, int[] dilation, int groups, bool benchmark, bool deterministic) -> Tensor
std::tuple<Tensor,Tensor,Tensor> miopen_convolution_backward(const Tensor & self, const Tensor & grad_output, const Tensor & weight, IntArrayRef padding, IntArrayRef stride, IntArrayRef dilation, int64_t groups, bool benchmark, bool deterministic, std::array<bool,3> output_mask); // aten::miopen_convolution_backward(Tensor self, Tensor grad_output, Tensor weight, int[] padding, int[] stride, int[] dilation, int groups, bool benchmark, bool deterministic, bool[3] output_mask) -> (Tensor, Tensor, Tensor)
Tensor miopen_convolution_backward_bias(const Tensor & grad_output); // aten::miopen_convolution_backward_bias(Tensor grad_output) -> Tensor
Tensor miopen_convolution_backward_input(IntArrayRef self_size, const Tensor & grad_output, const Tensor & weight, IntArrayRef padding, IntArrayRef stride, IntArrayRef dilation, int64_t groups, bool benchmark, bool deterministic); // aten::miopen_convolution_backward_input(int[] self_size, Tensor grad_output, Tensor weight, int[] padding, int[] stride, int[] dilation, int groups, bool benchmark, bool deterministic) -> Tensor
Tensor miopen_convolution_backward_weight(IntArrayRef weight_size, const Tensor & grad_output, const Tensor & self, IntArrayRef padding, IntArrayRef stride, IntArrayRef dilation, int64_t groups, bool benchmark, bool deterministic); // aten::miopen_convolution_backward_weight(int[] weight_size, Tensor grad_output, Tensor self, int[] padding, int[] stride, int[] dilation, int groups, bool benchmark, bool deterministic) -> Tensor
Tensor miopen_convolution_transpose(const Tensor & self, const Tensor & weight, const Tensor & bias, IntArrayRef padding, IntArrayRef output_padding, IntArrayRef stride, IntArrayRef dilation, int64_t groups, bool benchmark, bool deterministic); // aten::miopen_convolution_transpose(Tensor self, Tensor weight, Tensor? bias, int[] padding, int[] output_padding, int[] stride, int[] dilation, int groups, bool benchmark, bool deterministic) -> Tensor
std::tuple<Tensor,Tensor,Tensor> miopen_convolution_transpose_backward(const Tensor & self, const Tensor & grad_output, const Tensor & weight, IntArrayRef padding, IntArrayRef output_padding, IntArrayRef stride, IntArrayRef dilation, int64_t groups, bool benchmark, bool deterministic, std::array<bool,3> output_mask); // aten::miopen_convolution_transpose_backward(Tensor self, Tensor grad_output, Tensor weight, int[] padding, int[] output_padding, int[] stride, int[] dilation, int groups, bool benchmark, bool deterministic, bool[3] output_mask) -> (Tensor, Tensor, Tensor)
Tensor miopen_convolution_transpose_backward_input(const Tensor & grad_output, const Tensor & weight, IntArrayRef padding, IntArrayRef stride, IntArrayRef dilation, int64_t groups, bool benchmark, bool deterministic); // aten::miopen_convolution_transpose_backward_input(Tensor grad_output, Tensor weight, int[] padding, int[] stride, int[] dilation, int groups, bool benchmark, bool deterministic) -> Tensor
Tensor miopen_convolution_transpose_backward_weight(IntArrayRef weight_size, const Tensor & grad_output, const Tensor & self, IntArrayRef padding, IntArrayRef stride, IntArrayRef dilation, int64_t groups, bool benchmark, bool deterministic); // aten::miopen_convolution_transpose_backward_weight(int[] weight_size, Tensor grad_output, Tensor self, int[] padding, int[] stride, int[] dilation, int groups, bool benchmark, bool deterministic) -> Tensor
Tensor miopen_depthwise_convolution(const Tensor & self, const Tensor & weight, const Tensor & bias, IntArrayRef padding, IntArrayRef stride, IntArrayRef dilation, int64_t groups, bool benchmark, bool deterministic); // aten::miopen_depthwise_convolution(Tensor self, Tensor weight, Tensor? bias, int[] padding, int[] stride, int[] dilation, int groups, bool benchmark, bool deterministic) -> Tensor
std::tuple<Tensor,Tensor,Tensor> miopen_depthwise_convolution_backward(const Tensor & self, const Tensor & grad_output, const Tensor & weight, IntArrayRef padding, IntArrayRef stride, IntArrayRef dilation, int64_t groups, bool benchmark, bool deterministic, std::array<bool,3> output_mask); // aten::miopen_depthwise_convolution_backward(Tensor self, Tensor grad_output, Tensor weight, int[] padding, int[] stride, int[] dilation, int groups, bool benchmark, bool deterministic, bool[3] output_mask) -> (Tensor, Tensor, Tensor)
Tensor miopen_depthwise_convolution_backward_input(IntArrayRef self_size, const Tensor & grad_output, const Tensor & weight, IntArrayRef padding, IntArrayRef stride, IntArrayRef dilation, int64_t groups, bool benchmark, bool deterministic); // aten::miopen_depthwise_convolution_backward_input(int[] self_size, Tensor grad_output, Tensor weight, int[] padding, int[] stride, int[] dilation, int groups, bool benchmark, bool deterministic) -> Tensor
Tensor miopen_depthwise_convolution_backward_weight(IntArrayRef weight_size, const Tensor & grad_output, const Tensor & self, IntArrayRef padding, IntArrayRef stride, IntArrayRef dilation, int64_t groups, bool benchmark, bool deterministic); // aten::miopen_depthwise_convolution_backward_weight(int[] weight_size, Tensor grad_output, Tensor self, int[] padding, int[] stride, int[] dilation, int groups, bool benchmark, bool deterministic) -> Tensor
std::tuple<Tensor,Tensor,Tensor,Tensor,Tensor> miopen_rnn(const Tensor & input, TensorList weight, int64_t weight_stride0, const Tensor & hx, const Tensor & cx, int64_t mode, int64_t hidden_size, int64_t num_layers, bool batch_first, double dropout, bool train, bool bidirectional, IntArrayRef batch_sizes, const Tensor & dropout_state); // aten::miopen_rnn(Tensor input, Tensor[] weight, int weight_stride0, Tensor hx, Tensor? cx, int mode, int hidden_size, int num_layers, bool batch_first, float dropout, bool train, bool bidirectional, int[] batch_sizes, Tensor? dropout_state) -> (Tensor, Tensor, Tensor, Tensor, Tensor)
std::tuple<Tensor,Tensor,Tensor,std::vector<Tensor>> miopen_rnn_backward(const Tensor & input, TensorList weight, int64_t weight_stride0, const Tensor & weight_buf, const Tensor & hx, const Tensor & cx, const Tensor & output, const Tensor & grad_output, const Tensor & grad_hy, const Tensor & grad_cy, int64_t mode, int64_t hidden_size, int64_t num_layers, bool batch_first, double dropout, bool train, bool bidirectional, IntArrayRef batch_sizes, const Tensor & dropout_state, const Tensor & reserve, std::array<bool,4> output_mask); // aten::miopen_rnn_backward(Tensor input, Tensor[] weight, int weight_stride0, Tensor weight_buf, Tensor hx, Tensor? cx, Tensor output, Tensor? grad_output, Tensor? grad_hy, Tensor? grad_cy, int mode, int hidden_size, int num_layers, bool batch_first, float dropout, bool train, bool bidirectional, int[] batch_sizes, Tensor? dropout_state, Tensor reserve, bool[4] output_mask) -> (Tensor, Tensor, Tensor, Tensor[])
Tensor mkldnn_adaptive_avg_pool2d(const Tensor & self, IntArrayRef output_size); // aten::mkldnn_adaptive_avg_pool2d(Tensor self, int[2] output_size) -> Tensor
Tensor mkldnn_convolution(const Tensor & self, const Tensor & weight, const Tensor & bias, IntArrayRef padding, IntArrayRef stride, IntArrayRef dilation, int64_t groups); // aten::mkldnn_convolution(Tensor self, Tensor weight, Tensor? bias, int[] padding, int[] stride, int[] dilation, int groups) -> Tensor
std::tuple<Tensor,Tensor,Tensor> mkldnn_convolution_backward(const Tensor & self, const Tensor & grad_output, const Tensor & weight, IntArrayRef padding, IntArrayRef stride, IntArrayRef dilation, int64_t groups, std::array<bool,3> output_mask); // aten::mkldnn_convolution_backward(Tensor self, Tensor grad_output, Tensor weight, int[] padding, int[] stride, int[] dilation, int groups, bool[3] output_mask) -> (Tensor, Tensor, Tensor)
Tensor mkldnn_linear(const Tensor & input, const Tensor & weight, const Tensor & bias); // aten::mkldnn_linear(Tensor input, Tensor weight, Tensor? bias=None) -> Tensor
Tensor mkldnn_max_pool2d(const Tensor & self, IntArrayRef kernel_size, IntArrayRef stride, IntArrayRef padding, IntArrayRef dilation, bool ceil_mode); // aten::mkldnn_max_pool2d(Tensor self, int[2] kernel_size, int[2] stride=[], int[2] padding=0, int[2] dilation=1, bool ceil_mode=False) -> Tensor
Tensor mkldnn_reorder_conv2d_weight(const Tensor & self, IntArrayRef padding, IntArrayRef stride, IntArrayRef dilation, int64_t groups); // aten::mkldnn_reorder_conv2d_weight(Tensor self, int[2] padding=0, int[2] stride=1, int[2] dilation=1, int groups=1) -> Tensor
Tensor mm(const Tensor & self, const Tensor & mat2); // aten::mm(Tensor self, Tensor mat2) -> Tensor
Tensor & mm_out(Tensor & out, const Tensor & self, const Tensor & mat2); // aten::mm.out(Tensor self, Tensor mat2, *, Tensor(a!) out) -> Tensor(a!)
std::tuple<Tensor,Tensor> mode(const Tensor & self, int64_t dim, bool keepdim); // aten::mode(Tensor self, int dim=-1, bool keepdim=False) -> (Tensor values, Tensor indices)
std::tuple<Tensor &,Tensor &> mode_out(Tensor & values, Tensor & indices, const Tensor & self, int64_t dim, bool keepdim); // aten::mode.values(Tensor self, int dim=-1, bool keepdim=False, *, Tensor(a!) values, Tensor(b!) indices) -> (Tensor(a!) values, Tensor(b!) indices)
Tensor mse_loss(const Tensor & self, const Tensor & target, int64_t reduction); // aten::mse_loss(Tensor self, Tensor target, int reduction=Mean) -> Tensor
Tensor mse_loss_backward(const Tensor & grad_output, const Tensor & self, const Tensor & target, int64_t reduction); // aten::mse_loss_backward(Tensor grad_output, Tensor self, Tensor target, int reduction) -> Tensor
Tensor & mse_loss_backward_out(Tensor & grad_input, const Tensor & grad_output, const Tensor & self, const Tensor & target, int64_t reduction); // aten::mse_loss_backward.grad_input(Tensor grad_output, Tensor self, Tensor target, int reduction, *, Tensor(a!) grad_input) -> Tensor(a!)
Tensor & mse_loss_out(Tensor & out, const Tensor & self, const Tensor & target, int64_t reduction); // aten::mse_loss.out(Tensor self, Tensor target, int reduction=Mean, *, Tensor(a!) out) -> Tensor(a!)
Tensor mul(const Tensor & self, const Tensor & other); // aten::mul.Tensor(Tensor self, Tensor other) -> Tensor
Tensor mul(const Tensor & self, Scalar other); // aten::mul.Scalar(Tensor self, Scalar other) -> Tensor
Tensor & mul_(Tensor & self, const Tensor & other); // aten::mul_.Tensor(Tensor(a!) self, Tensor other) -> Tensor(a!)
Tensor & mul_(Tensor & self, Scalar other); // aten::mul_.Scalar(Tensor(a!) self, Scalar other) -> Tensor(a!)
Tensor & mul_out(Tensor & out, const Tensor & self, const Tensor & other); // aten::mul.out(Tensor self, Tensor other, *, Tensor(a!) out) -> Tensor(a!)
Tensor multi_margin_loss(const Tensor & self, const Tensor & target, Scalar p, Scalar margin, const Tensor & weight, int64_t reduction); // aten::multi_margin_loss(Tensor self, Tensor target, Scalar p=1, Scalar margin=1, Tensor? weight=None, int reduction=Mean) -> Tensor
Tensor multi_margin_loss_backward(const Tensor & grad_output, const Tensor & self, const Tensor & target, Scalar p, Scalar margin, const Tensor & weight, int64_t reduction); // aten::multi_margin_loss_backward(Tensor grad_output, Tensor self, Tensor target, Scalar p, Scalar margin, Tensor? weight=None, int reduction=Mean) -> Tensor
Tensor & multi_margin_loss_backward_out(Tensor & grad_input, const Tensor & grad_output, const Tensor & self, const Tensor & target, Scalar p, Scalar margin, const Tensor & weight, int64_t reduction); // aten::multi_margin_loss_backward.grad_input(Tensor grad_output, Tensor self, Tensor target, Scalar p, Scalar margin, Tensor? weight=None, int reduction=Mean, *, Tensor(a!) grad_input) -> Tensor(a!)
Tensor & multi_margin_loss_out(Tensor & out, const Tensor & self, const Tensor & target, Scalar p, Scalar margin, const Tensor & weight, int64_t reduction); // aten::multi_margin_loss.out(Tensor self, Tensor target, Scalar p=1, Scalar margin=1, Tensor? weight=None, int reduction=Mean, *, Tensor(a!) out) -> Tensor(a!)
Tensor multilabel_margin_loss_backward(const Tensor & grad_output, const Tensor & self, const Tensor & target, int64_t reduction, const Tensor & is_target); // aten::multilabel_margin_loss_backward(Tensor grad_output, Tensor self, Tensor target, int reduction, Tensor is_target) -> Tensor
Tensor & multilabel_margin_loss_backward_out(Tensor & grad_input, const Tensor & grad_output, const Tensor & self, const Tensor & target, int64_t reduction, const Tensor & is_target); // aten::multilabel_margin_loss_backward.grad_input(Tensor grad_output, Tensor self, Tensor target, int reduction, Tensor is_target, *, Tensor(a!) grad_input) -> Tensor(a!)
std::tuple<Tensor,Tensor> multilabel_margin_loss_forward(const Tensor & self, const Tensor & target, int64_t reduction); // aten::multilabel_margin_loss_forward(Tensor self, Tensor target, int reduction) -> (Tensor output, Tensor is_target)
std::tuple<Tensor &,Tensor &> multilabel_margin_loss_forward_out(Tensor & output, Tensor & is_target, const Tensor & self, const Tensor & target, int64_t reduction); // aten::multilabel_margin_loss_forward.output(Tensor self, Tensor target, int reduction, *, Tensor(a!) output, Tensor(b!) is_target) -> (Tensor(a!), Tensor(b!))
Tensor multinomial(const Tensor & self, int64_t num_samples, bool replacement, Generator * generator); // aten::multinomial(Tensor self, int num_samples, bool replacement=False, *, Generator? generator=None) -> Tensor
Tensor & multinomial_out(Tensor & out, const Tensor & self, int64_t num_samples, bool replacement, Generator * generator); // aten::multinomial.out(Tensor self, int num_samples, bool replacement=False, *, Generator? generator=None, Tensor(a!) out) -> Tensor(a!)
Tensor mv(const Tensor & self, const Tensor & vec); // aten::mv(Tensor self, Tensor vec) -> Tensor
Tensor & mv_out(Tensor & out, const Tensor & self, const Tensor & vec); // aten::mv.out(Tensor self, Tensor vec, *, Tensor(a!) out) -> Tensor(a!)
Tensor mvlgamma(const Tensor & self, int64_t p); // aten::mvlgamma(Tensor self, int p) -> Tensor
Tensor & mvlgamma_(Tensor & self, int64_t p); // aten::mvlgamma_(Tensor(a!) self, int p) -> Tensor(a!)
Tensor narrow_copy(const Tensor & self, int64_t dim, int64_t start, int64_t length); // aten::narrow_copy(Tensor self, int dim, int start, int length) -> Tensor
std::tuple<Tensor,Tensor,Tensor> native_batch_norm(const Tensor & input, const Tensor & weight, const Tensor & bias, const Tensor & running_mean, const Tensor & running_var, bool training, double momentum, double eps); // aten::native_batch_norm(Tensor input, Tensor? weight, Tensor? bias, Tensor? running_mean, Tensor? running_var, bool training, float momentum, float eps) -> (Tensor, Tensor, Tensor)
std::tuple<Tensor,Tensor,Tensor> native_batch_norm_backward(const Tensor & grad_out, const Tensor & input, const Tensor & weight, const Tensor & running_mean, const Tensor & running_var, const Tensor & save_mean, const Tensor & save_invstd, bool train, double eps, std::array<bool,3> output_mask); // aten::native_batch_norm_backward(Tensor grad_out, Tensor input, Tensor? weight, Tensor? running_mean, Tensor? running_var, Tensor? save_mean, Tensor? save_invstd, bool train, float eps, bool[3] output_mask) -> (Tensor, Tensor, Tensor)
std::tuple<Tensor,Tensor,Tensor> native_layer_norm(const Tensor & input, const Tensor & weight, const Tensor & bias, int64_t M, int64_t N, double eps); // aten::native_layer_norm(Tensor input, Tensor? weight, Tensor? bias, int M, int N, float eps) -> (Tensor, Tensor, Tensor)
std::tuple<Tensor,Tensor,Tensor> native_layer_norm_backward(const Tensor & grad_out, const Tensor & input, const Tensor & mean, const Tensor & rstd, const Tensor & weight, int64_t M, int64_t N, std::array<bool,3> output_mask); // aten::native_layer_norm_backward(Tensor grad_out, Tensor input, Tensor mean, Tensor rstd, Tensor? weight, int M, int N, bool[3] output_mask) -> (Tensor, Tensor, Tensor)
Tensor native_norm(const Tensor & self, Scalar p); // aten::native_norm(Tensor self, Scalar p=2) -> Tensor
Tensor ne(const Tensor & self, Scalar other); // aten::ne.Scalar(Tensor self, Scalar other) -> Tensor
Tensor ne(const Tensor & self, const Tensor & other); // aten::ne.Tensor(Tensor self, Tensor other) -> Tensor
Tensor & ne_(Tensor & self, Scalar other); // aten::ne_.Scalar(Tensor(a!) self, Scalar other) -> Tensor(a!)
Tensor & ne_(Tensor & self, const Tensor & other); // aten::ne_.Tensor(Tensor(a!) self, Tensor other) -> Tensor(a!)
Tensor & ne_out(Tensor & out, const Tensor & self, Scalar other); // aten::ne.Scalar_out(Tensor self, Scalar other, *, Tensor(a!) out) -> Tensor(a!)
Tensor & ne_out(Tensor & out, const Tensor & self, const Tensor & other); // aten::ne.Tensor_out(Tensor self, Tensor other, *, Tensor(a!) out) -> Tensor(a!)
Tensor neg(const Tensor & self); // aten::neg(Tensor self) -> Tensor
Tensor & neg_(Tensor & self); // aten::neg_(Tensor(a!) self) -> Tensor(a!)
Tensor & neg_out(Tensor & out, const Tensor & self); // aten::neg.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)
Tensor nll_loss2d_backward(const Tensor & grad_output, const Tensor & self, const Tensor & target, const Tensor & weight, int64_t reduction, int64_t ignore_index, const Tensor & total_weight); // aten::nll_loss2d_backward(Tensor grad_output, Tensor self, Tensor target, Tensor? weight, int reduction, int ignore_index, Tensor total_weight) -> Tensor
Tensor & nll_loss2d_backward_out(Tensor & grad_input, const Tensor & grad_output, const Tensor & self, const Tensor & target, const Tensor & weight, int64_t reduction, int64_t ignore_index, const Tensor & total_weight); // aten::nll_loss2d_backward.grad_input(Tensor grad_output, Tensor self, Tensor target, Tensor? weight, int reduction, int ignore_index, Tensor total_weight, *, Tensor(a!) grad_input) -> Tensor(a!)
std::tuple<Tensor,Tensor> nll_loss2d_forward(const Tensor & self, const Tensor & target, const Tensor & weight, int64_t reduction, int64_t ignore_index); // aten::nll_loss2d_forward(Tensor self, Tensor target, Tensor? weight, int reduction, int ignore_index) -> (Tensor output, Tensor total_weight)
std::tuple<Tensor &,Tensor &> nll_loss2d_forward_out(Tensor & output, Tensor & total_weight, const Tensor & self, const Tensor & target, const Tensor & weight, int64_t reduction, int64_t ignore_index); // aten::nll_loss2d_forward.output(Tensor self, Tensor target, Tensor? weight, int reduction, int ignore_index, *, Tensor(a!) output, Tensor(b!) total_weight) -> (Tensor(a!), Tensor(b!))
Tensor nll_loss_backward(const Tensor & grad_output, const Tensor & self, const Tensor & target, const Tensor & weight, int64_t reduction, int64_t ignore_index, const Tensor & total_weight); // aten::nll_loss_backward(Tensor grad_output, Tensor self, Tensor target, Tensor? weight, int reduction, int ignore_index, Tensor total_weight) -> Tensor
Tensor & nll_loss_backward_out(Tensor & grad_input, const Tensor & grad_output, const Tensor & self, const Tensor & target, const Tensor & weight, int64_t reduction, int64_t ignore_index, const Tensor & total_weight); // aten::nll_loss_backward.grad_input(Tensor grad_output, Tensor self, Tensor target, Tensor? weight, int reduction, int ignore_index, Tensor total_weight, *, Tensor(a!) grad_input) -> Tensor(a!)
std::tuple<Tensor,Tensor> nll_loss_forward(const Tensor & self, const Tensor & target, const Tensor & weight, int64_t reduction, int64_t ignore_index); // aten::nll_loss_forward(Tensor self, Tensor target, Tensor? weight, int reduction, int ignore_index) -> (Tensor output, Tensor total_weight)
std::tuple<Tensor &,Tensor &> nll_loss_forward_out(Tensor & output, Tensor & total_weight, const Tensor & self, const Tensor & target, const Tensor & weight, int64_t reduction, int64_t ignore_index); // aten::nll_loss_forward.output(Tensor self, Tensor target, Tensor? weight, int reduction, int ignore_index, *, Tensor(a!) output, Tensor(b!) total_weight) -> (Tensor(a!), Tensor(b!))
Tensor nonzero(const Tensor & self); // aten::nonzero(Tensor self) -> Tensor
Tensor & nonzero_out(Tensor & out, const Tensor & self); // aten::nonzero.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)
Tensor norm(const Tensor & self, c10::optional<Scalar> p, ScalarType dtype); // aten::norm.ScalarOpt_dtype(Tensor self, Scalar? p, *, ScalarType dtype) -> Tensor
Tensor norm(const Tensor & self, Scalar p); // aten::norm.Scalar(Tensor self, Scalar p=2) -> Tensor
Tensor norm(const Tensor & self, c10::optional<Scalar> p, IntArrayRef dim, bool keepdim, ScalarType dtype); // aten::norm.ScalarOpt_dim_dtype(Tensor self, Scalar? p, int[1] dim, bool keepdim, *, ScalarType dtype) -> Tensor
Tensor norm(const Tensor & self, c10::optional<Scalar> p, IntArrayRef dim, bool keepdim); // aten::norm.ScalarOpt_dim(Tensor self, Scalar? p, int[1] dim, bool keepdim=False) -> Tensor
Tensor & norm_out(Tensor & out, const Tensor & self, c10::optional<Scalar> p, IntArrayRef dim, bool keepdim, ScalarType dtype); // aten::norm.dtype_out(Tensor self, Scalar? p, int[1] dim, bool keepdim, *, ScalarType dtype, Tensor(a!) out) -> Tensor(a!)
Tensor & norm_out(Tensor & out, const Tensor & self, c10::optional<Scalar> p, IntArrayRef dim, bool keepdim); // aten::norm.out(Tensor self, Scalar? p, int[1] dim, bool keepdim=False, *, Tensor(a!) out) -> Tensor(a!)
Tensor normal(const Tensor & mean, double std, Generator * generator); // aten::normal.Tensor_float(Tensor mean, float std=1, *, Generator? generator=None) -> Tensor
Tensor normal(double mean, const Tensor & std, Generator * generator); // aten::normal.float_Tensor(float mean, Tensor std, *, Generator? generator=None) -> Tensor
Tensor normal(const Tensor & mean, const Tensor & std, Generator * generator); // aten::normal.Tensor_Tensor(Tensor mean, Tensor std, *, Generator? generator=None) -> Tensor
Tensor & normal_(Tensor & self, double mean, double std, Generator * generator); // aten::normal_(Tensor(a!) self, float mean=0, float std=1, *, Generator? generator=None) -> Tensor(a!)
Tensor & normal_out(Tensor & out, const Tensor & mean, double std, Generator * generator); // aten::normal.Tensor_float_out(Tensor mean, float std=1, *, Generator? generator=None, Tensor(a!) out) -> Tensor(a!)
Tensor & normal_out(Tensor & out, double mean, const Tensor & std, Generator * generator); // aten::normal.float_Tensor_out(float mean, Tensor std, *, Generator? generator=None, Tensor(a!) out) -> Tensor(a!)
Tensor & normal_out(Tensor & out, const Tensor & mean, const Tensor & std, Generator * generator); // aten::normal.Tensor_Tensor_out(Tensor mean, Tensor std, *, Generator? generator=None, Tensor(a!) out) -> Tensor(a!)
Tensor orgqr(const Tensor & self, const Tensor & input2); // aten::orgqr(Tensor self, Tensor input2) -> Tensor
Tensor & orgqr_out(Tensor & out, const Tensor & self, const Tensor & input2); // aten::orgqr.out(Tensor self, Tensor input2, *, Tensor(a!) out) -> Tensor(a!)
Tensor ormqr(const Tensor & self, const Tensor & input2, const Tensor & input3, bool left, bool transpose); // aten::ormqr(Tensor self, Tensor input2, Tensor input3, bool left=True, bool transpose=False) -> Tensor
Tensor & ormqr_out(Tensor & out, const Tensor & self, const Tensor & input2, const Tensor & input3, bool left, bool transpose); // aten::ormqr.out(Tensor self, Tensor input2, Tensor input3, bool left=True, bool transpose=False, *, Tensor(a!) out) -> Tensor(a!)
Tensor permute(const Tensor & self, IntArrayRef dims); // aten::permute(Tensor(a) self, int[] dims) -> Tensor(a)
Tensor poisson(const Tensor & self, Generator * generator); // aten::poisson(Tensor self, Generator? generator=None) -> Tensor
Tensor polygamma(int64_t n, const Tensor & self); // aten::polygamma(int n, Tensor self) -> Tensor
Tensor & polygamma_out(Tensor & out, int64_t n, const Tensor & self); // aten::polygamma.out(int n, Tensor self, *, Tensor(a!) out) -> Tensor(a!)
Tensor pow(const Tensor & self, Scalar exponent); // aten::pow.Tensor_Scalar(Tensor self, Scalar exponent) -> Tensor
Tensor pow(const Tensor & self, const Tensor & exponent); // aten::pow.Tensor_Tensor(Tensor self, Tensor exponent) -> Tensor
Tensor pow(Scalar self, const Tensor & exponent); // aten::pow.Scalar(Scalar self, Tensor exponent) -> Tensor
Tensor & pow_(Tensor & self, Scalar exponent); // aten::pow_.Scalar(Tensor(a!) self, Scalar exponent) -> Tensor(a!)
Tensor & pow_(Tensor & self, const Tensor & exponent); // aten::pow_.Tensor(Tensor(a!) self, Tensor exponent) -> Tensor(a!)
Tensor & pow_out(Tensor & out, const Tensor & self, Scalar exponent); // aten::pow.Tensor_Scalar_out(Tensor self, Scalar exponent, *, Tensor(a!) out) -> Tensor(a!)
Tensor & pow_out(Tensor & out, const Tensor & self, const Tensor & exponent); // aten::pow.Tensor_Tensor_out(Tensor self, Tensor exponent, *, Tensor(a!) out) -> Tensor(a!)
Tensor & pow_out(Tensor & out, Scalar self, const Tensor & exponent); // aten::pow.Scalar_out(Scalar self, Tensor exponent, *, Tensor(a!) out) -> Tensor(a!)
Tensor prelu(const Tensor & self, const Tensor & weight); // aten::prelu(Tensor self, Tensor weight) -> Tensor
std::tuple<Tensor,Tensor> prelu_backward(const Tensor & grad_output, const Tensor & self, const Tensor & weight); // aten::prelu_backward(Tensor grad_output, Tensor self, Tensor weight) -> (Tensor, Tensor)
Tensor prod(const Tensor & self, c10::optional<ScalarType> dtype); // aten::prod(Tensor self, *, ScalarType? dtype=None) -> Tensor
Tensor prod(const Tensor & self, int64_t dim, bool keepdim, c10::optional<ScalarType> dtype); // aten::prod.dim_int(Tensor self, int dim, bool keepdim=False, *, ScalarType? dtype=None) -> Tensor
Tensor & prod_out(Tensor & out, const Tensor & self, int64_t dim, bool keepdim, c10::optional<ScalarType> dtype); // aten::prod.int_out(Tensor self, int dim, bool keepdim=False, *, ScalarType? dtype=None, Tensor(a!) out) -> Tensor(a!)
Tensor & put_(Tensor & self, const Tensor & index, const Tensor & source, bool accumulate); // aten::put_(Tensor(a!) self, Tensor index, Tensor source, bool accumulate=False) -> Tensor(a!)
int64_t q_per_channel_axis(const Tensor & self); // aten::q_per_channel_axis(Tensor self) -> int
Tensor q_per_channel_scales(const Tensor & self); // aten::q_per_channel_scales(Tensor self) -> Tensor
Tensor q_per_channel_zero_points(const Tensor & self); // aten::q_per_channel_zero_points(Tensor self) -> Tensor
double q_scale(const Tensor & self); // aten::q_scale(Tensor self) -> float
int64_t q_zero_point(const Tensor & self); // aten::q_zero_point(Tensor self) -> int
std::tuple<Tensor,Tensor> qr(const Tensor & self, bool some); // aten::qr(Tensor self, bool some=True) -> (Tensor Q, Tensor R)
std::tuple<Tensor &,Tensor &> qr_out(Tensor & Q, Tensor & R, const Tensor & self, bool some); // aten::qr.Q(Tensor self, bool some=True, *, Tensor(a!) Q, Tensor(b!) R) -> (Tensor(a!) Q, Tensor(b!) R)
QScheme qscheme(const Tensor & self); // aten::qscheme(Tensor self) -> QScheme
Tensor quantize_per_channel(const Tensor & self, const Tensor & scales, const Tensor & zero_points, int64_t axis, ScalarType dtype); // aten::quantize_per_channel(Tensor self, Tensor scales, Tensor zero_points, int axis, ScalarType dtype) -> Tensor
Tensor quantize_per_tensor(const Tensor & self, double scale, int64_t zero_point, ScalarType dtype); // aten::quantize_per_tensor(Tensor self, float scale, int zero_point, ScalarType dtype) -> Tensor
Tensor quantized_max_pool2d(const Tensor & self, IntArrayRef kernel_size, IntArrayRef stride, IntArrayRef padding, IntArrayRef dilation, bool ceil_mode); // aten::quantized_max_pool2d(Tensor self, int[2] kernel_size, int[2] stride=[], int[2] padding=0, int[2] dilation=1, bool ceil_mode=False) -> Tensor
Tensor & random_(Tensor & self, int64_t from, int64_t to, Generator * generator); // aten::random_.from(Tensor(a!) self, int from, int to, *, Generator? generator=None) -> Tensor(a!)
Tensor & random_(Tensor & self, int64_t to, Generator * generator); // aten::random_.to(Tensor(a!) self, int to, *, Generator? generator=None) -> Tensor(a!)
Tensor & random_(Tensor & self, Generator * generator); // aten::random_(Tensor(a!) self, *, Generator? generator=None) -> Tensor(a!)
Tensor & randperm_out(Tensor & out, int64_t n, Generator * generator); // aten::randperm.generator_out(int n, *, Generator? generator, Tensor(a!) out) -> Tensor(a!)
Tensor & range_out(Tensor & out, Scalar start, Scalar end, Scalar step); // aten::range.out(Scalar start, Scalar end, Scalar step=1, *, Tensor(a!) out) -> Tensor(a!)
Tensor real(const Tensor & self); // aten::real(Tensor self) -> Tensor
Tensor & real_out(Tensor & out, const Tensor & self); // aten::real.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)
Tensor reciprocal(const Tensor & self); // aten::reciprocal(Tensor self) -> Tensor
Tensor & reciprocal_(Tensor & self); // aten::reciprocal_(Tensor(a!) self) -> Tensor(a!)
Tensor & reciprocal_out(Tensor & out, const Tensor & self); // aten::reciprocal.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)
Tensor reflection_pad1d(const Tensor & self, IntArrayRef padding); // aten::reflection_pad1d(Tensor self, int[2] padding) -> Tensor
Tensor reflection_pad1d_backward(const Tensor & grad_output, const Tensor & self, IntArrayRef padding); // aten::reflection_pad1d_backward(Tensor grad_output, Tensor self, int[2] padding) -> Tensor
Tensor & reflection_pad1d_backward_out(Tensor & grad_input, const Tensor & grad_output, const Tensor & self, IntArrayRef padding); // aten::reflection_pad1d_backward.grad_input(Tensor grad_output, Tensor self, int[2] padding, *, Tensor(a!) grad_input) -> Tensor(a!)
Tensor & reflection_pad1d_out(Tensor & out, const Tensor & self, IntArrayRef padding); // aten::reflection_pad1d.out(Tensor self, int[2] padding, *, Tensor(a!) out) -> Tensor(a!)
Tensor reflection_pad2d(const Tensor & self, IntArrayRef padding); // aten::reflection_pad2d(Tensor self, int[4] padding) -> Tensor
Tensor reflection_pad2d_backward(const Tensor & grad_output, const Tensor & self, IntArrayRef padding); // aten::reflection_pad2d_backward(Tensor grad_output, Tensor self, int[4] padding) -> Tensor
Tensor & reflection_pad2d_backward_out(Tensor & grad_input, const Tensor & grad_output, const Tensor & self, IntArrayRef padding); // aten::reflection_pad2d_backward.grad_input(Tensor grad_output, Tensor self, int[4] padding, *, Tensor(a!) grad_input) -> Tensor(a!)
Tensor & reflection_pad2d_out(Tensor & out, const Tensor & self, IntArrayRef padding); // aten::reflection_pad2d.out(Tensor self, int[4] padding, *, Tensor(a!) out) -> Tensor(a!)
Tensor relu(const Tensor & self); // aten::relu(Tensor self) -> Tensor
Tensor & relu_(Tensor & self); // aten::relu_(Tensor(a!) self) -> Tensor(a!)
Tensor remainder(const Tensor & self, Scalar other); // aten::remainder.Scalar(Tensor self, Scalar other) -> Tensor
Tensor remainder(const Tensor & self, const Tensor & other); // aten::remainder.Tensor(Tensor self, Tensor other) -> Tensor
Tensor & remainder_(Tensor & self, Scalar other); // aten::remainder_.Scalar(Tensor(a!) self, Scalar other) -> Tensor(a!)
Tensor & remainder_(Tensor & self, const Tensor & other); // aten::remainder_.Tensor(Tensor(a!) self, Tensor other) -> Tensor(a!)
Tensor & remainder_out(Tensor & out, const Tensor & self, Scalar other); // aten::remainder.Scalar_out(Tensor self, Scalar other, *, Tensor(a!) out) -> Tensor(a!)
Tensor & remainder_out(Tensor & out, const Tensor & self, const Tensor & other); // aten::remainder.Tensor_out(Tensor self, Tensor other, *, Tensor(a!) out) -> Tensor(a!)
Tensor renorm(const Tensor & self, Scalar p, int64_t dim, Scalar maxnorm); // aten::renorm(Tensor self, Scalar p, int dim, Scalar maxnorm) -> Tensor
Tensor & renorm_(Tensor & self, Scalar p, int64_t dim, Scalar maxnorm); // aten::renorm_(Tensor(a!) self, Scalar p, int dim, Scalar maxnorm) -> Tensor(a!)
Tensor & renorm_out(Tensor & out, const Tensor & self, Scalar p, int64_t dim, Scalar maxnorm); // aten::renorm.out(Tensor self, Scalar p, int dim, Scalar maxnorm, *, Tensor(a!) out) -> Tensor(a!)
Tensor repeat(const Tensor & self, IntArrayRef repeats); // aten::repeat(Tensor self, int[] repeats) -> Tensor
Tensor repeat_interleave(const Tensor & repeats); // aten::repeat_interleave.Tensor(Tensor repeats) -> Tensor
Tensor replication_pad1d(const Tensor & self, IntArrayRef padding); // aten::replication_pad1d(Tensor self, int[2] padding) -> Tensor
Tensor replication_pad1d_backward(const Tensor & grad_output, const Tensor & self, IntArrayRef padding); // aten::replication_pad1d_backward(Tensor grad_output, Tensor self, int[2] padding) -> Tensor
Tensor & replication_pad1d_backward_out(Tensor & grad_input, const Tensor & grad_output, const Tensor & self, IntArrayRef padding); // aten::replication_pad1d_backward.grad_input(Tensor grad_output, Tensor self, int[2] padding, *, Tensor(a!) grad_input) -> Tensor(a!)
Tensor & replication_pad1d_out(Tensor & out, const Tensor & self, IntArrayRef padding); // aten::replication_pad1d.out(Tensor self, int[2] padding, *, Tensor(a!) out) -> Tensor(a!)
Tensor replication_pad2d(const Tensor & self, IntArrayRef padding); // aten::replication_pad2d(Tensor self, int[4] padding) -> Tensor
Tensor replication_pad2d_backward(const Tensor & grad_output, const Tensor & self, IntArrayRef padding); // aten::replication_pad2d_backward(Tensor grad_output, Tensor self, int[4] padding) -> Tensor
Tensor & replication_pad2d_backward_out(Tensor & grad_input, const Tensor & grad_output, const Tensor & self, IntArrayRef padding); // aten::replication_pad2d_backward.grad_input(Tensor grad_output, Tensor self, int[4] padding, *, Tensor(a!) grad_input) -> Tensor(a!)
Tensor & replication_pad2d_out(Tensor & out, const Tensor & self, IntArrayRef padding); // aten::replication_pad2d.out(Tensor self, int[4] padding, *, Tensor(a!) out) -> Tensor(a!)
Tensor replication_pad3d(const Tensor & self, IntArrayRef padding); // aten::replication_pad3d(Tensor self, int[6] padding) -> Tensor
Tensor replication_pad3d_backward(const Tensor & grad_output, const Tensor & self, IntArrayRef padding); // aten::replication_pad3d_backward(Tensor grad_output, Tensor self, int[6] padding) -> Tensor
Tensor & replication_pad3d_backward_out(Tensor & grad_input, const Tensor & grad_output, const Tensor & self, IntArrayRef padding); // aten::replication_pad3d_backward.grad_input(Tensor grad_output, Tensor self, int[6] padding, *, Tensor(a!) grad_input) -> Tensor(a!)
Tensor & replication_pad3d_out(Tensor & out, const Tensor & self, IntArrayRef padding); // aten::replication_pad3d.out(Tensor self, int[6] padding, *, Tensor(a!) out) -> Tensor(a!)
Tensor & resize_(Tensor & self, IntArrayRef size, c10::optional<MemoryFormat> memory_format); // aten::resize_(Tensor(a!) self, int[] size, *, MemoryFormat? memory_format=None) -> Tensor(a!)
Tensor roll(const Tensor & self, IntArrayRef shifts, IntArrayRef dims); // aten::roll(Tensor self, int[1] shifts, int[1] dims=[]) -> Tensor
Tensor rot90(const Tensor & self, int64_t k, IntArrayRef dims); // aten::rot90(Tensor self, int k=1, int[] dims=[0,1]) -> Tensor
Tensor round(const Tensor & self); // aten::round(Tensor self) -> Tensor
Tensor & round_(Tensor & self); // aten::round_(Tensor(a!) self) -> Tensor(a!)
Tensor & round_out(Tensor & out, const Tensor & self); // aten::round.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)
Tensor rrelu_with_noise(const Tensor & self, const Tensor & noise, Scalar lower, Scalar upper, bool training, Generator * generator); // aten::rrelu_with_noise(Tensor self, Tensor noise, Scalar lower=0.125, Scalar upper=0.3333333333333333, bool training=False, Generator? generator=None) -> Tensor
Tensor & rrelu_with_noise_(Tensor & self, const Tensor & noise, Scalar lower, Scalar upper, bool training, Generator * generator); // aten::rrelu_with_noise_(Tensor(a!) self, Tensor noise, Scalar lower=0.125, Scalar upper=0.3333333333333333, bool training=False, Generator? generator=None) -> Tensor(a!)
Tensor rrelu_with_noise_backward(const Tensor & grad_output, const Tensor & self, const Tensor & noise, Scalar lower, Scalar upper, bool training); // aten::rrelu_with_noise_backward(Tensor grad_output, Tensor self, Tensor noise, Scalar lower, Scalar upper, bool training) -> Tensor
Tensor & rrelu_with_noise_backward_out(Tensor & grad_input, const Tensor & grad_output, const Tensor & self, const Tensor & noise, Scalar lower, Scalar upper, bool training); // aten::rrelu_with_noise_backward.grad_input(Tensor grad_output, Tensor self, Tensor noise, Scalar lower, Scalar upper, bool training, *, Tensor(a!) grad_input) -> Tensor(a!)
Tensor & rrelu_with_noise_out(Tensor & out, const Tensor & self, const Tensor & noise, Scalar lower, Scalar upper, bool training, Generator * generator); // aten::rrelu_with_noise.out(Tensor self, Tensor noise, Scalar lower=0.125, Scalar upper=0.3333333333333333, bool training=False, Generator? generator=None, *, Tensor(a!) out) -> Tensor(a!)
Tensor rsqrt(const Tensor & self); // aten::rsqrt(Tensor self) -> Tensor
Tensor & rsqrt_(Tensor & self); // aten::rsqrt_(Tensor(a!) self) -> Tensor(a!)
Tensor & rsqrt_out(Tensor & out, const Tensor & self); // aten::rsqrt.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)
Tensor rsub(const Tensor & self, const Tensor & other, Scalar alpha); // aten::rsub.Tensor(Tensor self, Tensor other, *, Scalar alpha=1) -> Tensor
Tensor rsub(const Tensor & self, Scalar other, Scalar alpha); // aten::rsub.Scalar(Tensor self, Scalar other, Scalar alpha=1) -> Tensor
Tensor & scatter_(Tensor & self, int64_t dim, const Tensor & index, const Tensor & src); // aten::scatter_.src(Tensor(a!) self, int dim, Tensor index, Tensor src) -> Tensor(a!)
Tensor & scatter_(Tensor & self, int64_t dim, const Tensor & index, Scalar value); // aten::scatter_.value(Tensor(a!) self, int dim, Tensor index, Scalar value) -> Tensor(a!)
Tensor & scatter_add_(Tensor & self, int64_t dim, const Tensor & index, const Tensor & src); // aten::scatter_add_(Tensor(a!) self, int dim, Tensor index, Tensor src) -> Tensor(a!)
Tensor select(const Tensor & self, int64_t dim, int64_t index); // aten::select.int(Tensor(a) self, int dim, int index) -> Tensor(a)
Tensor & set_(Tensor & self, Storage source); // aten::set_.source_Storage(Tensor(a!) self, Storage source) -> Tensor(a!)
Tensor & set_(Tensor & self, Storage source, int64_t storage_offset, IntArrayRef size, IntArrayRef stride); // aten::set_.source_Storage_storage_offset(Tensor(a!) self, Storage source, int storage_offset, int[] size, int[] stride=[]) -> Tensor(a!)
Tensor & set_(Tensor & self, const Tensor & source); // aten::set_.source_Tensor(Tensor(a!) self, Tensor source) -> Tensor(a!)
Tensor & set_(Tensor & self); // aten::set_(Tensor(a!) self) -> Tensor(a!)
Tensor & set_quantizer_(Tensor & self, ConstQuantizerPtr quantizer); // aten::set_quantizer_(Tensor(a!) self, ConstQuantizerPtr quantizer) -> Tensor(a!)
Tensor sigmoid(const Tensor & self); // aten::sigmoid(Tensor self) -> Tensor
Tensor & sigmoid_(Tensor & self); // aten::sigmoid_(Tensor(a!) self) -> Tensor(a!)
Tensor sigmoid_backward(const Tensor & grad_output, const Tensor & output); // aten::sigmoid_backward(Tensor grad_output, Tensor output) -> Tensor
Tensor & sigmoid_backward_out(Tensor & grad_input, const Tensor & grad_output, const Tensor & output); // aten::sigmoid_backward.grad_input(Tensor grad_output, Tensor output, *, Tensor(a!) grad_input) -> Tensor(a!)
Tensor & sigmoid_out(Tensor & out, const Tensor & self); // aten::sigmoid.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)
Tensor sign(const Tensor & self); // aten::sign(Tensor self) -> Tensor
Tensor & sign_(Tensor & self); // aten::sign_(Tensor(a!) self) -> Tensor(a!)
Tensor & sign_out(Tensor & out, const Tensor & self); // aten::sign.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)
Tensor sin(const Tensor & self); // aten::sin(Tensor self) -> Tensor
Tensor & sin_(Tensor & self); // aten::sin_(Tensor(a!) self) -> Tensor(a!)
Tensor & sin_out(Tensor & out, const Tensor & self); // aten::sin.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)
Tensor sinh(const Tensor & self); // aten::sinh(Tensor self) -> Tensor
Tensor & sinh_(Tensor & self); // aten::sinh_(Tensor(a!) self) -> Tensor(a!)
Tensor & sinh_out(Tensor & out, const Tensor & self); // aten::sinh.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)
Tensor slice(const Tensor & self, int64_t dim, int64_t start, int64_t end, int64_t step); // aten::slice.Tensor(Tensor(a) self, int dim=0, int start=0, int end=9223372036854775807, int step=1) -> Tensor(a)
std::tuple<Tensor,Tensor> slogdet(const Tensor & self); // aten::slogdet(Tensor self) -> (Tensor sign, Tensor logabsdet)
std::tuple<Tensor,Tensor,Tensor> slow_conv3d_backward(const Tensor & grad_output, const Tensor & self, const Tensor & weight, IntArrayRef kernel_size, IntArrayRef stride, IntArrayRef padding, const Tensor & finput, const Tensor & fgrad_input, std::array<bool,3> output_mask); // aten::slow_conv3d_backward.output_mask(Tensor grad_output, Tensor self, Tensor weight, int[3] kernel_size, int[3] stride, int[3] padding, Tensor finput, Tensor fgrad_input, bool[3] output_mask) -> (Tensor grad_input, Tensor grad_weight, Tensor grad_bias)
std::tuple<Tensor &,Tensor &,Tensor &> slow_conv3d_backward_out(Tensor & grad_input, Tensor & grad_weight, Tensor & grad_bias, const Tensor & grad_output, const Tensor & self, const Tensor & weight, IntArrayRef kernel_size, IntArrayRef stride, IntArrayRef padding, const Tensor & finput, const Tensor & fgrad_input); // aten::slow_conv3d_backward.grad_input(Tensor grad_output, Tensor self, Tensor weight, int[3] kernel_size, int[3] stride, int[3] padding, Tensor finput, Tensor fgrad_input, *, Tensor(a!)? grad_input, Tensor(b!)? grad_weight, Tensor(c!)? grad_bias) -> (Tensor(a!), Tensor(b!), Tensor(c!))
std::tuple<Tensor,Tensor,Tensor> slow_conv3d_forward(const Tensor & self, const Tensor & weight, IntArrayRef kernel_size, const Tensor & bias, IntArrayRef stride, IntArrayRef padding); // aten::slow_conv3d_forward(Tensor self, Tensor weight, int[3] kernel_size, Tensor? bias, int[3] stride, int[3] padding) -> (Tensor output, Tensor finput, Tensor fgrad_input)
std::tuple<Tensor &,Tensor &,Tensor &> slow_conv3d_forward_out(Tensor & output, Tensor & finput, Tensor & fgrad_input, const Tensor & self, const Tensor & weight, IntArrayRef kernel_size, const Tensor & bias, IntArrayRef stride, IntArrayRef padding); // aten::slow_conv3d_forward.output(Tensor self, Tensor weight, int[3] kernel_size, Tensor? bias, int[3] stride, int[3] padding, *, Tensor(a!) output, Tensor(b!) finput, Tensor(c!) fgrad_input) -> (Tensor(a!), Tensor(b!), Tensor(c!))
Tensor slow_conv_dilated2d(const Tensor & self, const Tensor & weight, IntArrayRef kernel_size, const Tensor & bias, IntArrayRef stride, IntArrayRef padding, IntArrayRef dilation); // aten::slow_conv_dilated2d(Tensor self, Tensor weight, int[2] kernel_size, Tensor? bias=None, int[2] stride=1, int[2] padding=0, int[2] dilation=1) -> Tensor
std::tuple<Tensor,Tensor,Tensor> slow_conv_dilated2d_backward(const Tensor & grad_output, const Tensor & self, const Tensor & weight, IntArrayRef kernel_size, IntArrayRef stride, IntArrayRef padding, IntArrayRef dilation, std::array<bool,3> output_mask); // aten::slow_conv_dilated2d_backward(Tensor grad_output, Tensor self, Tensor weight, int[2] kernel_size, int[2] stride, int[2] padding, int[2] dilation, bool[3] output_mask) -> (Tensor grad_input, Tensor grad_weight, Tensor grad_bias)
Tensor slow_conv_dilated3d(const Tensor & self, const Tensor & weight, IntArrayRef kernel_size, const Tensor & bias, IntArrayRef stride, IntArrayRef padding, IntArrayRef dilation); // aten::slow_conv_dilated3d(Tensor self, Tensor weight, int[3] kernel_size, Tensor? bias=None, int[3] stride=1, int[3] padding=0, int[3] dilation=1) -> Tensor
std::tuple<Tensor,Tensor,Tensor> slow_conv_dilated3d_backward(const Tensor & grad_output, const Tensor & self, const Tensor & weight, IntArrayRef kernel_size, IntArrayRef stride, IntArrayRef padding, IntArrayRef dilation, std::array<bool,3> output_mask); // aten::slow_conv_dilated3d_backward(Tensor grad_output, Tensor self, Tensor weight, int[3] kernel_size, int[3] stride, int[3] padding, int[3] dilation, bool[3] output_mask) -> (Tensor grad_input, Tensor grad_weight, Tensor grad_bias)
Tensor slow_conv_transpose2d(const Tensor & self, const Tensor & weight, IntArrayRef kernel_size, const Tensor & bias, IntArrayRef stride, IntArrayRef padding, IntArrayRef output_padding, IntArrayRef dilation); // aten::slow_conv_transpose2d(Tensor self, Tensor weight, int[2] kernel_size, Tensor? bias=None, int[2] stride=1, int[2] padding=0, int[2] output_padding=0, int[2] dilation=1) -> Tensor
std::tuple<Tensor,Tensor,Tensor> slow_conv_transpose2d_backward(const Tensor & grad_output, const Tensor & self, const Tensor & weight, IntArrayRef kernel_size, IntArrayRef stride, IntArrayRef padding, IntArrayRef output_padding, IntArrayRef dilation, const Tensor & columns, const Tensor & ones, std::array<bool,3> output_mask); // aten::slow_conv_transpose2d_backward.output_mask(Tensor grad_output, Tensor self, Tensor weight, int[2] kernel_size, int[2] stride, int[2] padding, int[2] output_padding, int[2] dilation, Tensor columns, Tensor ones, bool[3] output_mask) -> (Tensor grad_input, Tensor grad_weight, Tensor grad_bias)
std::tuple<Tensor &,Tensor &,Tensor &> slow_conv_transpose2d_backward_out(Tensor & grad_input, Tensor & grad_weight, Tensor & grad_bias, const Tensor & grad_output, const Tensor & self, const Tensor & weight, IntArrayRef kernel_size, IntArrayRef stride, IntArrayRef padding, IntArrayRef output_padding, IntArrayRef dilation, const Tensor & columns, const Tensor & ones); // aten::slow_conv_transpose2d_backward.grad_output(Tensor grad_output, Tensor self, Tensor weight, int[2] kernel_size, int[2] stride, int[2] padding, int[2] output_padding, int[2] dilation, Tensor columns, Tensor ones, *, Tensor(a!)? grad_input, Tensor(b!)? grad_weight, Tensor(c!)? grad_bias) -> (Tensor(a!), Tensor(b!), Tensor(c!))
Tensor & slow_conv_transpose2d_out(Tensor & out, const Tensor & self, const Tensor & weight, IntArrayRef kernel_size, const Tensor & bias, IntArrayRef stride, IntArrayRef padding, IntArrayRef output_padding, IntArrayRef dilation); // aten::slow_conv_transpose2d.out(Tensor self, Tensor weight, int[2] kernel_size, Tensor? bias=None, int[2] stride=1, int[2] padding=0, int[2] output_padding=0, int[2] dilation=1, *, Tensor(a!) out) -> Tensor(a!)
Tensor slow_conv_transpose3d(const Tensor & self, const Tensor & weight, IntArrayRef kernel_size, const Tensor & bias, IntArrayRef stride, IntArrayRef padding, IntArrayRef output_padding, IntArrayRef dilation); // aten::slow_conv_transpose3d(Tensor self, Tensor weight, int[3] kernel_size, Tensor? bias=None, int[3] stride=1, int[3] padding=0, int[3] output_padding=0, int[3] dilation=1) -> Tensor
std::tuple<Tensor,Tensor,Tensor> slow_conv_transpose3d_backward(const Tensor & grad_output, const Tensor & self, const Tensor & weight, IntArrayRef kernel_size, IntArrayRef stride, IntArrayRef padding, IntArrayRef output_padding, IntArrayRef dilation, const Tensor & finput, const Tensor & fgrad_input, std::array<bool,3> output_mask); // aten::slow_conv_transpose3d_backward.output_mask(Tensor grad_output, Tensor self, Tensor weight, int[3] kernel_size, int[3] stride, int[3] padding, int[3] output_padding, int[3] dilation, Tensor finput, Tensor fgrad_input, bool[3] output_mask) -> (Tensor grad_input, Tensor grad_weight, Tensor grad_bias)
std::tuple<Tensor &,Tensor &,Tensor &> slow_conv_transpose3d_backward_out(Tensor & grad_input, Tensor & grad_weight, Tensor & grad_bias, const Tensor & grad_output, const Tensor & self, const Tensor & weight, IntArrayRef kernel_size, IntArrayRef stride, IntArrayRef padding, IntArrayRef output_padding, IntArrayRef dilation, const Tensor & finput, const Tensor & fgrad_input); // aten::slow_conv_transpose3d_backward.grad_output(Tensor grad_output, Tensor self, Tensor weight, int[3] kernel_size, int[3] stride, int[3] padding, int[3] output_padding, int[3] dilation, Tensor finput, Tensor fgrad_input, *, Tensor(a!)? grad_input, Tensor(b!)? grad_weight, Tensor(c!)? grad_bias) -> (Tensor(a!), Tensor(b!), Tensor(c!))
Tensor & slow_conv_transpose3d_out(Tensor & out, const Tensor & self, const Tensor & weight, IntArrayRef kernel_size, const Tensor & bias, IntArrayRef stride, IntArrayRef padding, IntArrayRef output_padding, IntArrayRef dilation); // aten::slow_conv_transpose3d.out(Tensor self, Tensor weight, int[3] kernel_size, Tensor? bias=None, int[3] stride=1, int[3] padding=0, int[3] output_padding=0, int[3] dilation=1, *, Tensor(a!) out) -> Tensor(a!)
Tensor smooth_l1_loss(const Tensor & self, const Tensor & target, int64_t reduction); // aten::smooth_l1_loss(Tensor self, Tensor target, int reduction=Mean) -> Tensor
Tensor smooth_l1_loss_backward(const Tensor & grad_output, const Tensor & self, const Tensor & target, int64_t reduction); // aten::smooth_l1_loss_backward(Tensor grad_output, Tensor self, Tensor target, int reduction) -> Tensor
Tensor & smooth_l1_loss_backward_out(Tensor & grad_input, const Tensor & grad_output, const Tensor & self, const Tensor & target, int64_t reduction); // aten::smooth_l1_loss_backward.grad_input(Tensor grad_output, Tensor self, Tensor target, int reduction, *, Tensor(a!) grad_input) -> Tensor(a!)
Tensor & smooth_l1_loss_out(Tensor & out, const Tensor & self, const Tensor & target, int64_t reduction); // aten::smooth_l1_loss.out(Tensor self, Tensor target, int reduction=Mean, *, Tensor(a!) out) -> Tensor(a!)
Tensor soft_margin_loss(const Tensor & self, const Tensor & target, int64_t reduction); // aten::soft_margin_loss(Tensor self, Tensor target, int reduction=Mean) -> Tensor
Tensor soft_margin_loss_backward(const Tensor & grad_output, const Tensor & self, const Tensor & target, int64_t reduction); // aten::soft_margin_loss_backward(Tensor grad_output, Tensor self, Tensor target, int reduction) -> Tensor
Tensor & soft_margin_loss_backward_out(Tensor & grad_input, const Tensor & grad_output, const Tensor & self, const Tensor & target, int64_t reduction); // aten::soft_margin_loss_backward.grad_input(Tensor grad_output, Tensor self, Tensor target, int reduction, *, Tensor(a!) grad_input) -> Tensor(a!)
Tensor & soft_margin_loss_out(Tensor & out, const Tensor & self, const Tensor & target, int64_t reduction); // aten::soft_margin_loss.out(Tensor self, Tensor target, int reduction=Mean, *, Tensor(a!) out) -> Tensor(a!)
Tensor softplus(const Tensor & self, Scalar beta, Scalar threshold); // aten::softplus(Tensor self, Scalar beta=1, Scalar threshold=20) -> Tensor
Tensor softplus_backward(const Tensor & grad_output, const Tensor & self, Scalar beta, Scalar threshold, const Tensor & output); // aten::softplus_backward(Tensor grad_output, Tensor self, Scalar beta, Scalar threshold, Tensor output) -> Tensor
Tensor & softplus_backward_out(Tensor & grad_input, const Tensor & grad_output, const Tensor & self, Scalar beta, Scalar threshold, const Tensor & output); // aten::softplus_backward.grad_input(Tensor grad_output, Tensor self, Scalar beta, Scalar threshold, Tensor output, *, Tensor(a!) grad_input) -> Tensor(a!)
Tensor & softplus_out(Tensor & out, const Tensor & self, Scalar beta, Scalar threshold); // aten::softplus.out(Tensor self, Scalar beta=1, Scalar threshold=20, *, Tensor(a!) out) -> Tensor(a!)
Tensor softshrink(const Tensor & self, Scalar lambd); // aten::softshrink(Tensor self, Scalar lambd=0.5) -> Tensor
Tensor softshrink_backward(const Tensor & grad_output, const Tensor & self, Scalar lambd); // aten::softshrink_backward(Tensor grad_output, Tensor self, Scalar lambd) -> Tensor
Tensor & softshrink_backward_out(Tensor & grad_input, const Tensor & grad_output, const Tensor & self, Scalar lambd); // aten::softshrink_backward.grad_input(Tensor grad_output, Tensor self, Scalar lambd, *, Tensor(a!) grad_input) -> Tensor(a!)
Tensor & softshrink_out(Tensor & out, const Tensor & self, Scalar lambd); // aten::softshrink.out(Tensor self, Scalar lambd=0.5, *, Tensor(a!) out) -> Tensor(a!)
std::tuple<Tensor,Tensor> solve(const Tensor & self, const Tensor & A); // aten::solve(Tensor self, Tensor A) -> (Tensor solution, Tensor LU)
std::tuple<Tensor &,Tensor &> solve_out(Tensor & solution, Tensor & lu, const Tensor & self, const Tensor & A); // aten::solve.solution(Tensor self, Tensor A, *, Tensor(a!) solution, Tensor(b!) lu) -> (Tensor(a!) solution, Tensor(b!) LU)
std::tuple<Tensor,Tensor> sort(const Tensor & self, int64_t dim, bool descending); // aten::sort(Tensor self, int dim=-1, bool descending=False) -> (Tensor values, Tensor indices)
std::tuple<Tensor &,Tensor &> sort_out(Tensor & values, Tensor & indices, const Tensor & self, int64_t dim, bool descending); // aten::sort.values(Tensor self, int dim=-1, bool descending=False, *, Tensor(a!) values, Tensor(b!) indices) -> (Tensor(a!) values, Tensor(b!) indices)
int64_t sparse_dim(const Tensor & self); // aten::sparse_dim(Tensor self) -> int
Tensor sparse_mask(const Tensor & self, const Tensor & mask); // aten::sparse_mask(Tensor self, Tensor mask) -> Tensor
Tensor & sparse_resize_(Tensor & self, IntArrayRef size, int64_t sparse_dim, int64_t dense_dim); // aten::sparse_resize_(Tensor(a!) self, int[] size, int sparse_dim, int dense_dim) -> Tensor(a!)
Tensor & sparse_resize_and_clear_(Tensor & self, IntArrayRef size, int64_t sparse_dim, int64_t dense_dim); // aten::sparse_resize_and_clear_(Tensor(a!) self, int[] size, int sparse_dim, int dense_dim) -> Tensor(a!)
std::vector<Tensor> split(const Tensor & self, int64_t split_size, int64_t dim); // aten::split.Tensor(Tensor(a) self, int split_size, int dim=0) -> Tensor(a)[]
std::vector<Tensor> split_with_sizes(const Tensor & self, IntArrayRef split_sizes, int64_t dim); // aten::split_with_sizes(Tensor self, int[] split_sizes, int dim=0) -> Tensor[]
Tensor sqrt(const Tensor & self); // aten::sqrt(Tensor self) -> Tensor
Tensor & sqrt_(Tensor & self); // aten::sqrt_(Tensor(a!) self) -> Tensor(a!)
Tensor & sqrt_out(Tensor & out, const Tensor & self); // aten::sqrt.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)
Tensor squeeze(const Tensor & self); // aten::squeeze(Tensor(a) self) -> Tensor(a)
Tensor squeeze(const Tensor & self, int64_t dim); // aten::squeeze.dim(Tensor(a) self, int dim) -> Tensor(a)
Tensor & squeeze_(Tensor & self); // aten::squeeze_(Tensor(a!) self) -> Tensor(a!)
Tensor & squeeze_(Tensor & self, int64_t dim); // aten::squeeze_.dim(Tensor(a!) self, int dim) -> Tensor(a!)
Tensor & sspaddmm_out(Tensor & out, const Tensor & self, const Tensor & mat1, const Tensor & mat2, Scalar beta, Scalar alpha); // aten::sspaddmm.out(Tensor self, Tensor mat1, Tensor mat2, *, Scalar beta=1, Scalar alpha=1, Tensor(a!) out) -> Tensor(a!)
Tensor stack(TensorList tensors, int64_t dim); // aten::stack(Tensor[] tensors, int dim=0) -> Tensor
Tensor & stack_out(Tensor & out, TensorList tensors, int64_t dim); // aten::stack.out(Tensor[] tensors, int dim=0, *, Tensor(a!) out) -> Tensor(a!)
Tensor std(const Tensor & self, bool unbiased); // aten::std(Tensor self, bool unbiased=True) -> Tensor
Tensor std(const Tensor & self, IntArrayRef dim, bool unbiased, bool keepdim); // aten::std.dim(Tensor self, int[1] dim, bool unbiased=True, bool keepdim=False) -> Tensor
std::tuple<Tensor,Tensor> std_mean(const Tensor & self, bool unbiased); // aten::std_mean(Tensor self, bool unbiased=True) -> (Tensor, Tensor)
std::tuple<Tensor,Tensor> std_mean(const Tensor & self, IntArrayRef dim, bool unbiased, bool keepdim); // aten::std_mean.dim(Tensor self, int[1] dim, bool unbiased=True, bool keepdim=False) -> (Tensor, Tensor)
Tensor & std_out(Tensor & out, const Tensor & self, IntArrayRef dim, bool unbiased, bool keepdim); // aten::std.out(Tensor self, int[1] dim, bool unbiased=True, bool keepdim=False, *, Tensor(a!) out) -> Tensor(a!)
Tensor sub(const Tensor & self, const Tensor & other, Scalar alpha); // aten::sub.Tensor(Tensor self, Tensor other, *, Scalar alpha=1) -> Tensor
Tensor sub(const Tensor & self, Scalar other, Scalar alpha); // aten::sub.Scalar(Tensor self, Scalar other, Scalar alpha=1) -> Tensor
Tensor & sub_(Tensor & self, const Tensor & other, Scalar alpha); // aten::sub_.Tensor(Tensor(a!) self, Tensor other, *, Scalar alpha=1) -> Tensor(a!)
Tensor & sub_(Tensor & self, Scalar other, Scalar alpha); // aten::sub_.Scalar(Tensor(a!) self, Scalar other, Scalar alpha=1) -> Tensor(a!)
Tensor & sub_out(Tensor & out, const Tensor & self, const Tensor & other, Scalar alpha); // aten::sub.out(Tensor self, Tensor other, *, Scalar alpha=1, Tensor(a!) out) -> Tensor(a!)
Tensor sum(const Tensor & self, c10::optional<ScalarType> dtype); // aten::sum(Tensor self, *, ScalarType? dtype=None) -> Tensor
Tensor sum(const Tensor & self, IntArrayRef dim, bool keepdim, c10::optional<ScalarType> dtype); // aten::sum.dim_IntList(Tensor self, int[1] dim, bool keepdim=False, *, ScalarType? dtype=None) -> Tensor
Tensor & sum_out(Tensor & out, const Tensor & self, IntArrayRef dim, bool keepdim, c10::optional<ScalarType> dtype); // aten::sum.IntList_out(Tensor self, int[1] dim, bool keepdim=False, *, ScalarType? dtype=None, Tensor(a!) out) -> Tensor(a!)
std::tuple<Tensor,Tensor,Tensor> svd(const Tensor & self, bool some, bool compute_uv); // aten::svd(Tensor self, bool some=True, bool compute_uv=True) -> (Tensor U, Tensor S, Tensor V)
std::tuple<Tensor &,Tensor &,Tensor &> svd_out(Tensor & U, Tensor & S, Tensor & V, const Tensor & self, bool some, bool compute_uv); // aten::svd.U(Tensor self, bool some=True, bool compute_uv=True, *, Tensor(a!) U, Tensor(b!) S, Tensor(c!) V) -> (Tensor(a!) U, Tensor(b!) S, Tensor(c!) V)
std::tuple<Tensor,Tensor> symeig(const Tensor & self, bool eigenvectors, bool upper); // aten::symeig(Tensor self, bool eigenvectors=False, bool upper=True) -> (Tensor eigenvalues, Tensor eigenvectors)
std::tuple<Tensor &,Tensor &> symeig_out(Tensor & e, Tensor & V, const Tensor & self, bool eigenvectors, bool upper); // aten::symeig.e(Tensor self, bool eigenvectors=False, bool upper=True, *, Tensor(a!) e, Tensor(b!) V) -> (Tensor(a!) eigenvalues, Tensor(b!) eigenvectors)
Tensor t(const Tensor & self); // aten::t(Tensor(a) self) -> Tensor(a)
Tensor & t_(Tensor & self); // aten::t_(Tensor(a!) self) -> Tensor(a!)
Tensor take(const Tensor & self, const Tensor & index); // aten::take(Tensor self, Tensor index) -> Tensor
Tensor & take_out(Tensor & out, const Tensor & self, const Tensor & index); // aten::take.out(Tensor self, Tensor index, *, Tensor(a!) out) -> Tensor(a!)
Tensor tan(const Tensor & self); // aten::tan(Tensor self) -> Tensor
Tensor & tan_(Tensor & self); // aten::tan_(Tensor(a!) self) -> Tensor(a!)
Tensor & tan_out(Tensor & out, const Tensor & self); // aten::tan.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)
Tensor tanh(const Tensor & self); // aten::tanh(Tensor self) -> Tensor
Tensor & tanh_(Tensor & self); // aten::tanh_(Tensor(a!) self) -> Tensor(a!)
Tensor tanh_backward(const Tensor & grad_output, const Tensor & output); // aten::tanh_backward(Tensor grad_output, Tensor output) -> Tensor
Tensor & tanh_backward_out(Tensor & grad_input, const Tensor & grad_output, const Tensor & output); // aten::tanh_backward.grad_input(Tensor grad_output, Tensor output, *, Tensor(a!) grad_input) -> Tensor(a!)
Tensor & tanh_out(Tensor & out, const Tensor & self); // aten::tanh.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)
std::tuple<Tensor,Tensor,Tensor> thnn_conv2d_backward(const Tensor & grad_output, const Tensor & self, const Tensor & weight, IntArrayRef kernel_size, IntArrayRef stride, IntArrayRef padding, const Tensor & finput, const Tensor & fgrad_input, std::array<bool,3> output_mask); // aten::thnn_conv2d_backward.output_mask(Tensor grad_output, Tensor self, Tensor weight, int[2] kernel_size, int[2] stride, int[2] padding, Tensor finput, Tensor fgrad_input, bool[3] output_mask) -> (Tensor grad_input, Tensor grad_weight, Tensor grad_bias)
std::tuple<Tensor &,Tensor &,Tensor &> thnn_conv2d_backward_out(Tensor & grad_input, Tensor & grad_weight, Tensor & grad_bias, const Tensor & grad_output, const Tensor & self, const Tensor & weight, IntArrayRef kernel_size, IntArrayRef stride, IntArrayRef padding, const Tensor & finput, const Tensor & fgrad_input); // aten::thnn_conv2d_backward.grad_input(Tensor grad_output, Tensor self, Tensor weight, int[2] kernel_size, int[2] stride, int[2] padding, Tensor finput, Tensor fgrad_input, *, Tensor(a!)? grad_input, Tensor(b!)? grad_weight, Tensor(c!)? grad_bias) -> (Tensor(a!), Tensor(b!), Tensor(c!))
std::tuple<Tensor,Tensor,Tensor> thnn_conv2d_forward(const Tensor & self, const Tensor & weight, IntArrayRef kernel_size, const Tensor & bias, IntArrayRef stride, IntArrayRef padding); // aten::thnn_conv2d_forward(Tensor self, Tensor weight, int[2] kernel_size, Tensor? bias, int[2] stride, int[2] padding) -> (Tensor output, Tensor finput, Tensor fgrad_input)
std::tuple<Tensor &,Tensor &,Tensor &> thnn_conv2d_forward_out(Tensor & output, Tensor & finput, Tensor & fgrad_input, const Tensor & self, const Tensor & weight, IntArrayRef kernel_size, const Tensor & bias, IntArrayRef stride, IntArrayRef padding); // aten::thnn_conv2d_forward.output(Tensor self, Tensor weight, int[2] kernel_size, Tensor? bias, int[2] stride, int[2] padding, *, Tensor(a!) output, Tensor(b!) finput, Tensor(c!) fgrad_input) -> (Tensor(a!), Tensor(b!), Tensor(c!))
std::tuple<Tensor,Tensor> thnn_conv_depthwise2d_backward(const Tensor & grad_output, const Tensor & self, const Tensor & weight, IntArrayRef kernel_size, IntArrayRef stride, IntArrayRef padding, IntArrayRef dilation, std::array<bool,2> output_mask); // aten::thnn_conv_depthwise2d_backward.output_mask(Tensor grad_output, Tensor self, Tensor weight, int[2] kernel_size, int[2] stride, int[2] padding, int[2] dilation, bool[2] output_mask) -> (Tensor grad_input, Tensor grad_weight)
std::tuple<Tensor &,Tensor &> thnn_conv_depthwise2d_backward_out(Tensor & grad_input, Tensor & grad_weight, const Tensor & grad_output, const Tensor & self, const Tensor & weight, IntArrayRef kernel_size, IntArrayRef stride, IntArrayRef padding, IntArrayRef dilation); // aten::thnn_conv_depthwise2d_backward.grad_input(Tensor grad_output, Tensor self, Tensor weight, int[2] kernel_size, int[2] stride, int[2] padding, int[2] dilation, *, Tensor(a!)? grad_input, Tensor(b!)? grad_weight) -> (Tensor(a!), Tensor(b!))
Tensor thnn_conv_depthwise2d_forward(const Tensor & self, const Tensor & weight, IntArrayRef kernel_size, const Tensor & bias, IntArrayRef stride, IntArrayRef padding, IntArrayRef dilation); // aten::thnn_conv_depthwise2d_forward(Tensor self, Tensor weight, int[2] kernel_size, Tensor? bias, int[2] stride, int[2] padding, int[2] dilation) -> Tensor
Tensor & thnn_conv_depthwise2d_forward_out(Tensor & out, const Tensor & self, const Tensor & weight, IntArrayRef kernel_size, const Tensor & bias, IntArrayRef stride, IntArrayRef padding, IntArrayRef dilation); // aten::thnn_conv_depthwise2d_forward.out(Tensor self, Tensor weight, int[2] kernel_size, Tensor? bias, int[2] stride, int[2] padding, int[2] dilation, *, Tensor(a!) out) -> Tensor(a!)
Tensor threshold(const Tensor & self, Scalar threshold, Scalar value); // aten::threshold(Tensor self, Scalar threshold, Scalar value) -> Tensor
Tensor & threshold_(Tensor & self, Scalar threshold, Scalar value); // aten::threshold_(Tensor(a!) self, Scalar threshold, Scalar value) -> Tensor(a!)
Tensor threshold_backward(const Tensor & grad_output, const Tensor & self, Scalar threshold); // aten::threshold_backward(Tensor grad_output, Tensor self, Scalar threshold) -> Tensor
Tensor & threshold_out(Tensor & out, const Tensor & self, Scalar threshold, Scalar value); // aten::threshold.out(Tensor self, Scalar threshold, Scalar value, *, Tensor(a!) out) -> Tensor(a!)
Tensor to_dense(const Tensor & self); // aten::to_dense(Tensor self) -> Tensor
Tensor to_mkldnn(const Tensor & self); // aten::to_mkldnn(Tensor self) -> Tensor
Tensor to_sparse(const Tensor & self, int64_t sparse_dim); // aten::to_sparse.sparse_dim(Tensor self, int sparse_dim) -> Tensor
Tensor to_sparse(const Tensor & self); // aten::to_sparse(Tensor self) -> Tensor
std::tuple<Tensor,Tensor> topk(const Tensor & self, int64_t k, int64_t dim, bool largest, bool sorted); // aten::topk(Tensor self, int k, int dim=-1, bool largest=True, bool sorted=True) -> (Tensor values, Tensor indices)
std::tuple<Tensor &,Tensor &> topk_out(Tensor & values, Tensor & indices, const Tensor & self, int64_t k, int64_t dim, bool largest, bool sorted); // aten::topk.values(Tensor self, int k, int dim=-1, bool largest=True, bool sorted=True, *, Tensor(a!) values, Tensor(b!) indices) ->(Tensor(a!) values, Tensor(b!) indices)
Tensor trace(const Tensor & self); // aten::trace(Tensor self) -> Tensor
Tensor transpose(const Tensor & self, int64_t dim0, int64_t dim1); // aten::transpose.int(Tensor(a) self, int dim0, int dim1) -> Tensor(a)
Tensor & transpose_(Tensor & self, int64_t dim0, int64_t dim1); // aten::transpose_(Tensor(a!) self, int dim0, int dim1) -> Tensor(a!)
std::tuple<Tensor,Tensor> triangular_solve(const Tensor & self, const Tensor & A, bool upper, bool transpose, bool unitriangular); // aten::triangular_solve(Tensor self, Tensor A, bool upper=True, bool transpose=False, bool unitriangular=False) -> (Tensor solution, Tensor cloned_coefficient)
std::tuple<Tensor &,Tensor &> triangular_solve_out(Tensor & X, Tensor & M, const Tensor & self, const Tensor & A, bool upper, bool transpose, bool unitriangular); // aten::triangular_solve.X(Tensor self, Tensor A, bool upper=True, bool transpose=False, bool unitriangular=False, *, Tensor(a!) X, Tensor(b!) M) -> (Tensor(a!) solution, Tensor(b!) cloned_coefficient)
Tensor tril(const Tensor & self, int64_t diagonal); // aten::tril(Tensor self, int diagonal=0) -> Tensor
Tensor & tril_(Tensor & self, int64_t diagonal); // aten::tril_(Tensor(a!) self, int diagonal=0) -> Tensor(a!)
Tensor tril_indices(int64_t row, int64_t col, int64_t offset, const TensorOptions & options); // aten::tril_indices(int row, int col, int offset=0, *, ScalarType? dtype=long, Layout? layout=None, Device? device=None, bool? pin_memory=None) -> Tensor
Tensor & tril_out(Tensor & out, const Tensor & self, int64_t diagonal); // aten::tril.out(Tensor self, int diagonal=0, *, Tensor(a!) out) -> Tensor(a!)
Tensor triu(const Tensor & self, int64_t diagonal); // aten::triu(Tensor self, int diagonal=0) -> Tensor
Tensor & triu_(Tensor & self, int64_t diagonal); // aten::triu_(Tensor(a!) self, int diagonal=0) -> Tensor(a!)
Tensor triu_indices(int64_t row, int64_t col, int64_t offset, const TensorOptions & options); // aten::triu_indices(int row, int col, int offset=0, *, ScalarType? dtype=long, Layout? layout=None, Device? device=None, bool? pin_memory=None) -> Tensor
Tensor & triu_out(Tensor & out, const Tensor & self, int64_t diagonal); // aten::triu.out(Tensor self, int diagonal=0, *, Tensor(a!) out) -> Tensor(a!)
Tensor trunc(const Tensor & self); // aten::trunc(Tensor self) -> Tensor
Tensor & trunc_(Tensor & self); // aten::trunc_(Tensor(a!) self) -> Tensor(a!)
Tensor & trunc_out(Tensor & out, const Tensor & self); // aten::trunc.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)
std::vector<Tensor> unbind(const Tensor & self, int64_t dim); // aten::unbind.int(Tensor(a) self, int dim=0) -> Tensor(a)[]
Tensor unfold(const Tensor & self, int64_t dimension, int64_t size, int64_t step); // aten::unfold(Tensor(a) self, int dimension, int size, int step) -> Tensor(a)
Tensor & uniform_(Tensor & self, double from, double to, Generator * generator); // aten::uniform_(Tensor(a!) self, float from=0, float to=1, *, Generator? generator=None) -> Tensor(a!)
std::tuple<Tensor,Tensor,Tensor> unique_consecutive(const Tensor & self, bool return_inverse, bool return_counts, c10::optional<int64_t> dim); // aten::unique_consecutive(Tensor self, bool return_inverse=False, bool return_counts=False, int? dim=None) -> (Tensor, Tensor, Tensor)
std::tuple<Tensor,Tensor,Tensor> unique_dim(const Tensor & self, int64_t dim, bool sorted, bool return_inverse, bool return_counts); // aten::unique_dim(Tensor self, int dim, bool sorted=True, bool return_inverse=False, bool return_counts=False) -> (Tensor, Tensor, Tensor)
std::tuple<Tensor,Tensor,Tensor> unique_dim_consecutive(const Tensor & self, int64_t dim, bool return_inverse, bool return_counts); // aten::unique_dim_consecutive(Tensor self, int dim, bool return_inverse=False, bool return_counts=False) -> (Tensor, Tensor, Tensor)
Tensor unsqueeze(const Tensor & self, int64_t dim); // aten::unsqueeze(Tensor(a) self, int dim) -> Tensor(a)
Tensor & unsqueeze_(Tensor & self, int64_t dim); // aten::unsqueeze_(Tensor(a!) self, int dim) -> Tensor(a!)
Tensor upsample_bicubic2d(const Tensor & self, IntArrayRef output_size, bool align_corners); // aten::upsample_bicubic2d(Tensor self, int[2] output_size, bool align_corners) -> Tensor
Tensor upsample_bicubic2d_backward(const Tensor & grad_output, IntArrayRef output_size, IntArrayRef input_size, bool align_corners); // aten::upsample_bicubic2d_backward(Tensor grad_output, int[2] output_size, int[4] input_size, bool align_corners) -> Tensor
Tensor & upsample_bicubic2d_backward_out(Tensor & grad_input, const Tensor & grad_output, IntArrayRef output_size, IntArrayRef input_size, bool align_corners); // aten::upsample_bicubic2d_backward.grad_input(Tensor grad_output, int[2] output_size, int[4] input_size, bool align_corners, *, Tensor(a!) grad_input) -> Tensor(a!)
Tensor & upsample_bicubic2d_out(Tensor & out, const Tensor & self, IntArrayRef output_size, bool align_corners); // aten::upsample_bicubic2d.out(Tensor self, int[2] output_size, bool align_corners, *, Tensor(a!) out) -> Tensor(a!)
Tensor upsample_bilinear2d(const Tensor & self, IntArrayRef output_size, bool align_corners); // aten::upsample_bilinear2d(Tensor self, int[2] output_size, bool align_corners) -> Tensor
Tensor upsample_bilinear2d_backward(const Tensor & grad_output, IntArrayRef output_size, IntArrayRef input_size, bool align_corners); // aten::upsample_bilinear2d_backward(Tensor grad_output, int[2] output_size, int[4] input_size, bool align_corners) -> Tensor
Tensor & upsample_bilinear2d_backward_out(Tensor & grad_input, const Tensor & grad_output, IntArrayRef output_size, IntArrayRef input_size, bool align_corners); // aten::upsample_bilinear2d_backward.grad_input(Tensor grad_output, int[2] output_size, int[4] input_size, bool align_corners, *, Tensor(a!) grad_input) -> Tensor(a!)
Tensor & upsample_bilinear2d_out(Tensor & out, const Tensor & self, IntArrayRef output_size, bool align_corners); // aten::upsample_bilinear2d.out(Tensor self, int[2] output_size, bool align_corners, *, Tensor(a!) out) -> Tensor(a!)
Tensor upsample_linear1d(const Tensor & self, IntArrayRef output_size, bool align_corners); // aten::upsample_linear1d(Tensor self, int[1] output_size, bool align_corners) -> Tensor
Tensor upsample_linear1d_backward(const Tensor & grad_output, IntArrayRef output_size, IntArrayRef input_size, bool align_corners); // aten::upsample_linear1d_backward(Tensor grad_output, int[1] output_size, int[3] input_size, bool align_corners) -> Tensor
Tensor & upsample_linear1d_backward_out(Tensor & grad_input, const Tensor & grad_output, IntArrayRef output_size, IntArrayRef input_size, bool align_corners); // aten::upsample_linear1d_backward.grad_input(Tensor grad_output, int[1] output_size, int[3] input_size, bool align_corners, *, Tensor(a!) grad_input) -> Tensor(a!)
Tensor & upsample_linear1d_out(Tensor & out, const Tensor & self, IntArrayRef output_size, bool align_corners); // aten::upsample_linear1d.out(Tensor self, int[1] output_size, bool align_corners, *, Tensor(a!) out) -> Tensor(a!)
Tensor upsample_nearest1d(const Tensor & self, IntArrayRef output_size); // aten::upsample_nearest1d(Tensor self, int[1] output_size) -> Tensor
Tensor upsample_nearest1d_backward(const Tensor & grad_output, IntArrayRef output_size, IntArrayRef input_size); // aten::upsample_nearest1d_backward(Tensor grad_output, int[1] output_size, int[3] input_size) -> Tensor
Tensor & upsample_nearest1d_backward_out(Tensor & grad_input, const Tensor & grad_output, IntArrayRef output_size, IntArrayRef input_size); // aten::upsample_nearest1d_backward.grad_input(Tensor grad_output, int[1] output_size, int[3] input_size, *, Tensor(a!) grad_input) -> Tensor(a!)
Tensor & upsample_nearest1d_out(Tensor & out, const Tensor & self, IntArrayRef output_size); // aten::upsample_nearest1d.out(Tensor self, int[1] output_size, *, Tensor(a!) out) -> Tensor(a!)
Tensor upsample_nearest2d(const Tensor & self, IntArrayRef output_size); // aten::upsample_nearest2d(Tensor self, int[2] output_size) -> Tensor
Tensor upsample_nearest2d_backward(const Tensor & grad_output, IntArrayRef output_size, IntArrayRef input_size); // aten::upsample_nearest2d_backward(Tensor grad_output, int[2] output_size, int[4] input_size) -> Tensor
Tensor & upsample_nearest2d_backward_out(Tensor & grad_input, const Tensor & grad_output, IntArrayRef output_size, IntArrayRef input_size); // aten::upsample_nearest2d_backward.grad_input(Tensor grad_output, int[2] output_size, int[4] input_size, *, Tensor(a!) grad_input) -> Tensor(a!)
Tensor & upsample_nearest2d_out(Tensor & out, const Tensor & self, IntArrayRef output_size); // aten::upsample_nearest2d.out(Tensor self, int[2] output_size, *, Tensor(a!) out) -> Tensor(a!)
Tensor upsample_nearest3d(const Tensor & self, IntArrayRef output_size); // aten::upsample_nearest3d(Tensor self, int[3] output_size) -> Tensor
Tensor upsample_nearest3d_backward(const Tensor & grad_output, IntArrayRef output_size, IntArrayRef input_size); // aten::upsample_nearest3d_backward(Tensor grad_output, int[3] output_size, int[5] input_size) -> Tensor
Tensor & upsample_nearest3d_backward_out(Tensor & grad_input, const Tensor & grad_output, IntArrayRef output_size, IntArrayRef input_size); // aten::upsample_nearest3d_backward.grad_input(Tensor grad_output, int[3] output_size, int[5] input_size, *, Tensor(a!) grad_input) -> Tensor(a!)
Tensor & upsample_nearest3d_out(Tensor & out, const Tensor & self, IntArrayRef output_size); // aten::upsample_nearest3d.out(Tensor self, int[3] output_size, *, Tensor(a!) out) -> Tensor(a!)
Tensor upsample_trilinear3d(const Tensor & self, IntArrayRef output_size, bool align_corners); // aten::upsample_trilinear3d(Tensor self, int[3] output_size, bool align_corners) -> Tensor
Tensor upsample_trilinear3d_backward(const Tensor & grad_output, IntArrayRef output_size, IntArrayRef input_size, bool align_corners); // aten::upsample_trilinear3d_backward(Tensor grad_output, int[3] output_size, int[5] input_size, bool align_corners) -> Tensor
Tensor & upsample_trilinear3d_backward_out(Tensor & grad_input, const Tensor & grad_output, IntArrayRef output_size, IntArrayRef input_size, bool align_corners); // aten::upsample_trilinear3d_backward.grad_input(Tensor grad_output, int[3] output_size, int[5] input_size, bool align_corners, *, Tensor(a!) grad_input) -> Tensor(a!)
Tensor & upsample_trilinear3d_out(Tensor & out, const Tensor & self, IntArrayRef output_size, bool align_corners); // aten::upsample_trilinear3d.out(Tensor self, int[3] output_size, bool align_corners, *, Tensor(a!) out) -> Tensor(a!)
Tensor values(const Tensor & self); // aten::values(Tensor(a) self) -> Tensor(a)
Tensor var(const Tensor & self, bool unbiased); // aten::var(Tensor self, bool unbiased=True) -> Tensor
Tensor var(const Tensor & self, IntArrayRef dim, bool unbiased, bool keepdim); // aten::var.dim(Tensor self, int[1] dim, bool unbiased=True, bool keepdim=False) -> Tensor
std::tuple<Tensor,Tensor> var_mean(const Tensor & self, bool unbiased); // aten::var_mean(Tensor self, bool unbiased=True) -> (Tensor, Tensor)
std::tuple<Tensor,Tensor> var_mean(const Tensor & self, IntArrayRef dim, bool unbiased, bool keepdim); // aten::var_mean.dim(Tensor self, int[1] dim, bool unbiased=True, bool keepdim=False) -> (Tensor, Tensor)
Tensor & var_out(Tensor & out, const Tensor & self, IntArrayRef dim, bool unbiased, bool keepdim); // aten::var.out(Tensor self, int[1] dim, bool unbiased=True, bool keepdim=False, *, Tensor(a!) out) -> Tensor(a!)
Tensor view(const Tensor & self, IntArrayRef size); // aten::view(Tensor(a) self, int[] size) -> Tensor(a)
Tensor & zero_(Tensor & self); // aten::zero_(Tensor(a!) self) -> Tensor(a!)
