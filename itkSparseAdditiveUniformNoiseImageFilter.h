#ifndef __itkSparseAdditiveUniformNoiseImageFilter
#define __itkSparseAdditiveUniformNoiseImageFilter

#include <itkUnaryFunctorImageFilter.h>
#include <itkConceptChecking.h>
#include <vnl/vnl_sample.h>

namespace itk
{
namespace Functor
{
/** \class SparseAdditiveUniformNoise
 * \brief Randomly adds additive uniform noise to a pixel.
 * The probability to alter a pixel is user-defined.
 * \ingroup ITKImageIntensity
 */
template< class TInput, class TOutput >
class SparseAdditiveUniformNoise
{
public:
  SparseAdditiveUniformNoise()
    {
    m_Probability = 1.0;
    m_Mean = 0.0;
    m_Amplitude = 1.0;
    m_OutputMinimum = itk::NumericTraits< TOutput >::NonpositiveMin();
    m_OutputMaximum = itk::NumericTraits< TOutput >::max();
    }

  ~SparseAdditiveUniformNoise() {}

  double GetProbability() const
    { return m_Probability; }

  void SetProbability(const double probability)
    {
    if(probability < 0.0 || probability > 1.0)
      itkGenericExceptionMacro("probability must be between 0 and 1");

    m_Probability = probability;
    }

  double GetMean() const
    { return m_Mean; }

  double GetAmplitude() const
    { return m_Amplitude; }

  void SetMean(const double mean)
    {
    m_Mean = mean;
    ComputeNoiseRange();
    }

  void SetAmplitude(const double amplitude)
    {
    if(amplitude < 0.0)
      itkGenericExceptionMacro("amplitude must be positive");

    m_Amplitude = amplitude;
    ComputeNoiseRange();
    }

  TOutput GetOutputMinimum() const
    { return m_OutputMinimum; }

  TOutput GetOutputMaximum() const
    { return m_OutputMaximum; }

  void SetOutputBounds( const TOutput min, const TOutput max)
    {
    if(max <= min)
      itkGenericExceptionMacro("invalid bounds: [" << min << "; " << max << "]");

    m_OutputMinimum = min;
    m_OutputMaximum = max;
    }

  bool operator!=(const SparseAdditiveUniformNoise &other) const
    {
    return m_Probability != other.m_Probability
      || m_Mean != other.m_Mean
      || m_Amplitude != other.m_Amplitude
      || m_OutputMaximum != other.m_OutputMaximum
      || m_OutputMinimum != other.m_OutputMinimum;
    }

  bool operator==(const SparseAdditiveUniformNoise & other) const
    {
    return !( *this != other );
    }

  inline TOutput operator()(const TInput & A) const
    {
    if(vnl_sample_uniform(0., 1.) <= m_Probability)
      {
      const double v = A + vnl_sample_uniform(m_NoiseMin, m_NoiseMax);
      return static_cast<TOutput>(v < m_OutputMinimum ? m_OutputMinimum : (v > m_OutputMaximum ? m_OutputMaximum : v));
      }
    else
      return static_cast<TOutput>(A);
    }

private:
  void ComputeNoiseRange()
    {
    m_NoiseMin = m_Mean - m_Amplitude;
    m_NoiseMax = m_Mean + m_Amplitude;
    }

private:
  TOutput m_OutputMinimum;
  TOutput m_OutputMaximum;
  double m_Probability;
  double m_Mean;
  double m_Amplitude;

  double m_NoiseMin;
  double m_NoiseMax;
};
} // End namespace Functor

template< class TInputImage, class TOutputImage >
class ITK_EXPORT SparseAdditiveUniformNoiseImageFilter:
  public
  UnaryFunctorImageFilter< TInputImage, TOutputImage,
                           Functor::SparseAdditiveUniformNoise<
                             typename TInputImage::PixelType,
                             typename TOutputImage::PixelType > >
{
public:
  /** Standard class typedefs. */
  typedef SparseAdditiveUniformNoiseImageFilter Self;
  typedef UnaryFunctorImageFilter<
    TInputImage, TOutputImage,
    Functor::SparseAdditiveUniformNoise< typename TInputImage::PixelType,
                                    typename TOutputImage::PixelType > >  Superclass;

  typedef SmartPointer< Self >       Pointer;
  typedef SmartPointer< const Self > ConstPointer;

  typedef typename TOutputImage::PixelType OutputPixelType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Macro that provides the GetNameOfClass() method */
  itkTypeMacro(SparseAdditiveUniformNoiseImageFilter, UnaryFunctorImageFilter);

  OutputPixelType GetOutputMinimum() const
    {
    return this->GetFunctor().GetOutputMinimum();
    }

  OutputPixelType GetOutputMaximum() const
    {
    return this->GetFunctor().GetOutputMaximum();
    }

  float GetProbability() const
    { return this->GetFunctor().GetProbability(); }

  double GetMean() const
    { return this->GetFunctor().GetMean(); }

  double GetAmplitude() const
    { return this->GetFunctor().GetAmplitude(); }

  void SetOutputBounds(const OutputPixelType min, const OutputPixelType max)
    {
    if ( min == this->GetFunctor().GetOutputMinimum() && max == this->GetFunctor().GetOutputMaximum())
      {
      return;
      }

    this->GetFunctor().SetOutputBounds(min, max);
    this->Modified();
    }

  void SetProbability(const double probability)
    {
    if ( probability == this->GetFunctor().GetProbability() )
      {
      return;
      }
    this->GetFunctor().SetProbability(probability);
    this->Modified();
    }

  void SetMean(const double mean)
    {
    if ( mean == this->GetFunctor().GetMean() )
      {
      return;
      }

    this->GetFunctor().SetMean(mean);
    this->Modified();
    }

  void SetAmplitude(const double amplitude)
    {
    if ( amplitude == this->GetFunctor().GetAmplitude() )
      {
      return;
      }

    this->GetFunctor().SetAmplitude(amplitude);
    this->Modified();
    }

  void PrintSelf(std::ostream& os, Indent indent) const
    {
    Superclass::PrintSelf(os, indent);
    os << indent << "Probability: " << static_cast<typename NumericTraits<double>::PrintType>(GetProbability()) << std::endl;
    os << indent << "Mean: " << static_cast<typename NumericTraits<double>::PrintType>(GetMean()) << std::endl;
    os << indent << "Amplitude: " << static_cast<typename NumericTraits<double>::PrintType>(GetAmplitude()) << std::endl;
    os << indent << "OutputMinimum: " << static_cast<typename NumericTraits<double>::PrintType>(GetOutputMinimum()) << std::endl;
    os << indent << "OutputMaximum: " << static_cast<typename NumericTraits<double>::PrintType>(GetOutputMaximum()) << std::endl;
    }

#ifdef ITK_USE_CONCEPT_CHECKING
  itkConceptMacro( InputConvertibleToDoubleCheck,
    ( Concept::Convertible< typename TInputImage::PixelType, double > ) );
  itkConceptMacro( DoubleConvertibleToOutputCheck,
    ( Concept::Convertible< double, OutputPixelType > ) );
#endif

protected:
  SparseAdditiveUniformNoiseImageFilter() {}
  virtual ~SparseAdditiveUniformNoiseImageFilter() {}

private:
  SparseAdditiveUniformNoiseImageFilter(const Self &); //purposely not implemented
  void operator=(const Self &);     //purposely not implemented
};

} // End namespace itk

#endif /* __itkSparseAdditiveUniformNoiseImageFilter */
