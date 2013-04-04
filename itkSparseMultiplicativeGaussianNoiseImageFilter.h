#ifndef __itkSparseMultiplicativeGaussianNoiseImageFilter
#define __itkSparseMultiplicativeGaussianNoiseImageFilter

#include <itkUnaryFunctorImageFilter.h>
#include <itkConceptChecking.h>
#include <vnl/vnl_sample.h>

namespace itk
{
namespace Functor
{
/** \class SparseMultiplicativeGaussianNoise
 * \brief Randomly adds multiplicative gaussian noise to a pixel.
 * The probability to alter a pixel is user-defined.
 * \ingroup ITKImageIntensity
 */
template< class TInput, class TOutput >
class SparseMultiplicativeGaussianNoise
{
public:
  SparseMultiplicativeGaussianNoise()
    {
    m_Probability = 1.0;
    m_Mean = 0.1;
    m_StandardDeviation = 1.0;
    m_OutputMinimum = itk::NumericTraits< TOutput >::NonpositiveMin();
    m_OutputMaximum = itk::NumericTraits< TOutput >::max();
    }

  ~SparseMultiplicativeGaussianNoise() {}

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

  double GetStandardDeviation() const
    { return m_StandardDeviation; }

  void SetMean(const double mean)
    { m_Mean = mean; }

  void SetStandardDeviation(const double standardDeviation)
    {
    if(standardDeviation <= 0.0)
      itkGenericExceptionMacro("standard deviation must be strictly positive");

    m_StandardDeviation = standardDeviation;
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

  bool operator!=(const SparseMultiplicativeGaussianNoise &other) const
    {
    return m_Probability != other.m_Probability
      || m_Mean != other.m_Mean
      || m_StandardDeviation != other.m_StandardDeviation
      || m_OutputMaximum != other.m_OutputMaximum
      || m_OutputMinimum != other.m_OutputMinimum;
    }

  bool operator==(const SparseMultiplicativeGaussianNoise & other) const
    {
    return !( *this != other );
    }

  inline TOutput operator()(const TInput & A) const
    {
    if(vnl_sample_uniform(0., 1.) <= m_Probability)
      {
      const double v = A * vnl_sample_normal(m_Mean, m_StandardDeviation);
      return static_cast<TOutput>(v < m_OutputMinimum ? m_OutputMinimum : (v > m_OutputMaximum ? m_OutputMaximum : v));
      }
    else
      return static_cast<TOutput>(A);
    }

private:
  TOutput m_OutputMinimum;
  TOutput m_OutputMaximum;
  double m_Probability;
  double m_Mean;
  double m_StandardDeviation;
};
} // End namespace Functor

template< class TInputImage, class TOutputImage >
class ITK_EXPORT SparseMultiplicativeGaussianNoiseImageFilter:
  public
  UnaryFunctorImageFilter< TInputImage, TOutputImage,
                           Functor::SparseMultiplicativeGaussianNoise<
                             typename TInputImage::PixelType,
                             typename TOutputImage::PixelType > >
{
public:
  /** Standard class typedefs. */
  typedef SparseMultiplicativeGaussianNoiseImageFilter Self;
  typedef UnaryFunctorImageFilter<
    TInputImage, TOutputImage,
    Functor::SparseMultiplicativeGaussianNoise< typename TInputImage::PixelType,
                                    typename TOutputImage::PixelType > >  Superclass;

  typedef SmartPointer< Self >       Pointer;
  typedef SmartPointer< const Self > ConstPointer;

  typedef typename TOutputImage::PixelType OutputPixelType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Macro that provides the GetNameOfClass() method */
  itkTypeMacro(SparseMultiplicativeGaussianNoiseImageFilter, UnaryFunctorImageFilter);

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

  double GetStandardDeviation() const
    { return this->GetFunctor().GetStandardDeviation(); }

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

  void SetStandardDeviation(const double standardDeviation)
    {
    if ( standardDeviation == this->GetFunctor().GetStandardDeviation() )
      {
      return;
      }

    this->GetFunctor().SetStandardDeviation(standardDeviation);
    this->Modified();
    }

  void PrintSelf(std::ostream& os, Indent indent) const
    {
    Superclass::PrintSelf(os, indent);
    os << indent << "Probability: " << static_cast<typename NumericTraits<double>::PrintType>(GetProbability()) << std::endl;
    os << indent << "Mean: " << static_cast<typename NumericTraits<double>::PrintType>(GetMean()) << std::endl;
    os << indent << "StandardDeviation: " << static_cast<typename NumericTraits<double>::PrintType>(GetStandardDeviation()) << std::endl;
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
  SparseMultiplicativeGaussianNoiseImageFilter() {}
  virtual ~SparseMultiplicativeGaussianNoiseImageFilter() {}

private:
  SparseMultiplicativeGaussianNoiseImageFilter(const Self &); //purposely not implemented
  void operator=(const Self &);     //purposely not implemented
};

} // End namespace itk

#endif /* __itkSparseMultiplicativeGaussianNoiseImageFilter */
