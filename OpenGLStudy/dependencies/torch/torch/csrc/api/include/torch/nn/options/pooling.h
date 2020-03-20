#pragma once

#include <torch/arg.h>
#include <torch/csrc/WindowsTorchApiMacro.h>
#include <torch/expanding_array.h>
#include <torch/nn/options/common.h>
#include <torch/types.h>

namespace torch {
namespace nn {

/// Options for a `D`-dimensional avgpool functional and module.
template <size_t D>
struct AvgPoolOptions {
  AvgPoolOptions(ExpandingArray<D> kernel_size)
      : kernel_size_(kernel_size), stride_(kernel_size) {}

  /// the size of the window to take an average over
  TORCH_ARG(ExpandingArray<D>, kernel_size);

  /// the stride of the window. Default value is `kernel_size`
  TORCH_ARG(ExpandingArray<D>, stride);

  /// implicit zero padding to be added on both sides
  TORCH_ARG(ExpandingArray<D>, padding) = 0;

  /// when True, will use `ceil` instead of `floor` to compute the output shape
  TORCH_ARG(bool, ceil_mode) = false;

  /// when True, will include the zero-padding in the averaging calculation
  TORCH_ARG(bool, count_include_pad) = true;

  /// if specified, it will be used as divisor, otherwise `kernel_size` will be used
  TORCH_ARG(c10::optional<int64_t>, divisor_override) = c10::nullopt;
};

/// `AvgPoolOptions` specialized for 1-D avgpool.
using AvgPool1dOptions = AvgPoolOptions<1>;

/// `AvgPoolOptions` specialized for 2-D avgpool.
using AvgPool2dOptions = AvgPoolOptions<2>;

/// `AvgPoolOptions` specialized for 3-D avgpool.
using AvgPool3dOptions = AvgPoolOptions<3>;

TORCH_NN_FUNCTIONAL_USE_MODULE_OPTIONS(AvgPool1d, AvgPool1dFuncOptions)
TORCH_NN_FUNCTIONAL_USE_MODULE_OPTIONS(AvgPool2d, AvgPool2dFuncOptions)
TORCH_NN_FUNCTIONAL_USE_MODULE_OPTIONS(AvgPool3d, AvgPool3dFuncOptions)

// ============================================================================

/// Options for a `D`-dimensional maxpool functional and module.
template <size_t D>
struct MaxPoolOptions {
  MaxPoolOptions(ExpandingArray<D> kernel_size)
      : kernel_size_(kernel_size), stride_(kernel_size) {}

  /// the size of the window to take a max over
  TORCH_ARG(ExpandingArray<D>, kernel_size);

  /// the stride of the window. Default value is `kernel_size
  TORCH_ARG(ExpandingArray<D>, stride);

  /// implicit zero padding to be added on both sides
  TORCH_ARG(ExpandingArray<D>, padding) = 0;

  /// a parameter that controls the stride of elements in the window
  TORCH_ARG(ExpandingArray<D>, dilation) = 1;

  /// when True, will use `ceil` instead of `floor` to compute the output shape
  TORCH_ARG(bool, ceil_mode) = false;
};

/// `MaxPoolOptions` specialized for 1-D maxpool.
using MaxPool1dOptions = MaxPoolOptions<1>;

/// `MaxPoolOptions` specialized for 2-D maxpool.
using MaxPool2dOptions = MaxPoolOptions<2>;

/// `MaxPoolOptions` specialized for 3-D maxpool.
using MaxPool3dOptions = MaxPoolOptions<3>;

TORCH_NN_FUNCTIONAL_USE_MODULE_OPTIONS(MaxPool1d, MaxPool1dFuncOptions)
TORCH_NN_FUNCTIONAL_USE_MODULE_OPTIONS(MaxPool2d, MaxPool2dFuncOptions)
TORCH_NN_FUNCTIONAL_USE_MODULE_OPTIONS(MaxPool3d, MaxPool3dFuncOptions)

// ============================================================================

/// Options for a `D`-dimensional adaptive maxpool functional and module.
template <size_t D>
struct AdaptiveMaxPoolOptions {
  AdaptiveMaxPoolOptions(ExpandingArray<D> output_size)
      : output_size_(output_size) {}

  /// the target output size
  TORCH_ARG(ExpandingArray<D>, output_size);
};

/// `AdaptiveMaxPoolOptions` specialized for 1-D maxpool.
using AdaptiveMaxPool1dOptions = AdaptiveMaxPoolOptions<1>;

/// `AdaptiveMaxPoolOptions` specialized for 2-D adaptive maxpool.
using AdaptiveMaxPool2dOptions = AdaptiveMaxPoolOptions<2>;

/// `AdaptiveMaxPoolOptions` specialized for 3-D adaptive maxpool.
using AdaptiveMaxPool3dOptions = AdaptiveMaxPoolOptions<3>;

TORCH_NN_FUNCTIONAL_USE_MODULE_OPTIONS(AdaptiveMaxPool1d, AdaptiveMaxPool1dFuncOptions)
TORCH_NN_FUNCTIONAL_USE_MODULE_OPTIONS(AdaptiveMaxPool2d, AdaptiveMaxPool2dFuncOptions)
TORCH_NN_FUNCTIONAL_USE_MODULE_OPTIONS(AdaptiveMaxPool3d, AdaptiveMaxPool3dFuncOptions)

// ============================================================================

/// Options for a `D`-dimensional adaptive avgpool functional and module.
template <size_t D>
struct AdaptiveAvgPoolOptions {
  AdaptiveAvgPoolOptions(ExpandingArray<D> output_size)
      : output_size_(output_size) {}

  /// the target output size
  TORCH_ARG(ExpandingArray<D>, output_size);
};

/// `AdaptiveAvgPoolOptions` specialized for 1-D adaptive avgpool.
using AdaptiveAvgPool1dOptions = AdaptiveAvgPoolOptions<1>;

/// `AdaptiveAvgPoolOptions` specialized for 2-D adaptive avgpool.
using AdaptiveAvgPool2dOptions = AdaptiveAvgPoolOptions<2>;

/// `AdaptiveAvgPoolOptions` specialized for 3-D adaptive avgpool.
using AdaptiveAvgPool3dOptions = AdaptiveAvgPoolOptions<3>;

TORCH_NN_FUNCTIONAL_USE_MODULE_OPTIONS(AdaptiveAvgPool1d, AdaptiveAvgPool1dFuncOptions)
TORCH_NN_FUNCTIONAL_USE_MODULE_OPTIONS(AdaptiveAvgPool2d, AdaptiveAvgPool2dFuncOptions)
TORCH_NN_FUNCTIONAL_USE_MODULE_OPTIONS(AdaptiveAvgPool3d, AdaptiveAvgPool3dFuncOptions)

// ============================================================================

/// Options for a `D`-dimensional maxunpool module.
template <size_t D>
struct MaxUnpoolOptions {
  MaxUnpoolOptions(ExpandingArray<D> kernel_size)
      : kernel_size_(kernel_size), stride_(kernel_size) {}

  /// the size of the window to take a max over
  TORCH_ARG(ExpandingArray<D>, kernel_size);

  /// the stride of the window. Default value is `kernel_size
  TORCH_ARG(ExpandingArray<D>, stride);

  /// implicit zero padding to be added on both sides
  TORCH_ARG(ExpandingArray<D>, padding) = 0;
};

/// `MaxUnpoolOptions` specialized for 1-D maxunpool.
using MaxUnpool1dOptions = MaxUnpoolOptions<1>;

/// `MaxUnpoolOptions` specialized for 2-D maxunpool.
using MaxUnpool2dOptions = MaxUnpoolOptions<2>;

/// `MaxUnpoolOptions` specialized for 3-D maxunpool.
using MaxUnpool3dOptions = MaxUnpoolOptions<3>;

// ============================================================================

namespace functional {

/// Options for a `D`-dimensional maxunpool functional.
template <size_t D>
struct MaxUnpoolFuncOptions {
  MaxUnpoolFuncOptions(ExpandingArray<D> kernel_size)
      : kernel_size_(kernel_size), stride_(kernel_size) {}

  /// the size of the window to take a max over
  TORCH_ARG(ExpandingArray<D>, kernel_size);

  /// the stride of the window. Default value is `kernel_size
  TORCH_ARG(ExpandingArray<D>, stride);

  /// implicit zero padding to be added on both sides
  TORCH_ARG(ExpandingArray<D>, padding) = 0;

  /// the targeted output size
  TORCH_ARG(c10::optional<std::vector<int64_t>>, output_size) = c10::nullopt;
};

/// `MaxUnpoolFuncOptions` specialized for 1-D maxunpool.
using MaxUnpool1dFuncOptions = MaxUnpoolFuncOptions<1>;

/// `MaxUnpoolFuncOptions` specialized for 2-D maxunpool.
using MaxUnpool2dFuncOptions = MaxUnpoolFuncOptions<2>;

/// `MaxUnpoolFuncOptions` specialized for 3-D maxunpool.
using MaxUnpool3dFuncOptions = MaxUnpoolFuncOptions<3>;

} // namespace functional

// ============================================================================

/// Options for a `D`-dimensional fractional maxpool functional and module.
template <size_t D>
struct FractionalMaxPoolOptions {
  FractionalMaxPoolOptions(ExpandingArray<D> kernel_size)
      : kernel_size_(kernel_size) {}

  /// the size of the window to take a max over
  TORCH_ARG(ExpandingArray<D>, kernel_size);

  /// the target output size of the image
  TORCH_ARG(c10::optional<ExpandingArray<D>>, output_size) = c10::nullopt;

  /// If one wants to have an output size as a ratio of the input size, this option can be given.
  /// This has to be a number or tuple in the range (0, 1)
  TORCH_ARG(c10::optional<ExpandingArray<D>>, output_ratio) = c10::nullopt;

  TORCH_ARG(torch::Tensor, _random_samples) = Tensor();
};

/// `FractionalMaxPoolOptions` specialized for 2-D maxpool.
using FractionalMaxPool2dOptions = FractionalMaxPoolOptions<2>;

/// `FractionalMaxPoolOptions` specialized for 3-D maxpool.
using FractionalMaxPool3dOptions = FractionalMaxPoolOptions<3>;

TORCH_NN_FUNCTIONAL_USE_MODULE_OPTIONS(FractionalMaxPool2d, FractionalMaxPool2dFuncOptions)
TORCH_NN_FUNCTIONAL_USE_MODULE_OPTIONS(FractionalMaxPool3d, FractionalMaxPool3dFuncOptions)

// ============================================================================

/// Options for a `D`-dimensional lppool functional and module.
template <size_t D>
struct LPPoolOptions {
  LPPoolOptions(double norm_type, ExpandingArray<D> kernel_size)
      : norm_type_(norm_type), kernel_size_(kernel_size), stride_(kernel_size) {}

  TORCH_ARG(double, norm_type);

  // the size of the window to take an average over
  TORCH_ARG(ExpandingArray<D>, kernel_size);

  // the stride of the window. Default value is `kernel_size`
  TORCH_ARG(ExpandingArray<D>, stride);

  // when True, will use `ceil` instead of `floor` to compute the output shape
  TORCH_ARG(bool, ceil_mode) = false;
};

/// `LPPoolOptions` specialized for 1-D lppool.
using LPPool1dOptions = LPPoolOptions<1>;

/// `LPPoolOptions` specialized for 2-D lppool.
using LPPool2dOptions = LPPoolOptions<2>;

TORCH_NN_FUNCTIONAL_USE_MODULE_OPTIONS(LPPool1d, LPPool1dFuncOptions)
TORCH_NN_FUNCTIONAL_USE_MODULE_OPTIONS(LPPool2d, LPPool2dFuncOptions)

} // namespace nn
} // namespace torch
