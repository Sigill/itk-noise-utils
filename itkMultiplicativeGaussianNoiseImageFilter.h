#ifndef __itkMultiplicativeGaussianNoiseImageFilter
#define __itkMultiplicativeGaussianNoiseImageFilter

#include <itkUnaryFunctorImageFilter.h>
#include <itkConceptChecking.h>
#include <vnl/vnl_sample.h>

namespace itk
{
namespace Functor
{
/** \class MultiplicativeGaussianNoise
 * \brief Adds multiplicative gaussian noise to a pixel.
 * \ingroup ITKImageIntensity
 */
template< class TInput, class TOutput >
class MultiplicativeGaussianNoise
{
public:
  MultiplicativeGaussianNoise()
    {
    m_Mean = 1.0;
    m_StandardDeviation = 1.0;
    m_OutputMinimum = itk::NumericTraits< TOutput >::NonpositiveMin();
    m_OutputMaximum = itk::NumericTraits< TOutput >::max();
    }

  ~MultiplicativeGaussianNoise() {}

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

  bool operator!=(const MultiplicativeGaussianNoise &other) const
    {
    return m_Mean != other.m_Mean
      || m_StandardDeviation != other.m_StandardDeviation
      || m_OutputMaximum != other.m_OutputMaximum
      || m_OutputMinimum != other.m_OutputMinimum;
    }

  bool operator==(const MultiplicativeGaussianNoise & other) const
    {
    return !( *this != other );
    }

  inline TOutput operator()(const TInput & A) const
    {
    double v = A * vnl_sample_normal(m_Mean, m_StandardDeviation);
    return static_cast<TOutput>(v < m_OutputMinimum ? m_OutputMinimum : (v > m_OutputMaximum ? m_OutputMaximum : v));
    }

private:
  TOutput m_OutputMinimum;
  TOutput m_OutputMaximum;
  double m_Mean;
  double m_StandardDeviation;
};
} // End namespace Functor

template< class TInputImage, class TOutputImage >
class ITK_EXPORT MultiplicativeGaussianNoiseImageFilter:
  public
  UnaryFunctorImageFilter< TInputImage, TOutputImage,
                           Functor::MultiplicativeGaussianNoise<
                             typename TInputImage::PixelType,
                             typename TOutputImage::PixelType > >
{
public:
  /** Standard class typedefs. */
  typedef MultiplicativeGaussianNoiseImageFilter Self;
  typedef UnaryFunctorImageFilter<
    TInputImage, TOutputImage,
    Functor::MultiplicativeGaussianNoise< typename TInputImage::PixelType,
                                    typename TOutputImage::PixelType > >  Superclass;

  typedef SmartPointer< Self >       Pointer;
  typedef SmartPointer< const Self > ConstPointer;

  typedef typename TOutputImage::PixelType OutputPixelType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Macro that provides the GetNameOfClass() method */
  itkTypeMacro(MultiplicativeGaussianNoiseImageFilter, UnaryFunctorImageFilter);

  OutputPixelType GetOutputMinimum() const
    {
    return this->GetFunctor().GetOutputMinimum();
    }

  OutputPixelType GetOutputMaximum() const
    {
    return this->GetFunctor().GetOutputMaximum();
    }

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
  MultiplicativeGaussianNoiseImageFilter() {}
  virtual ~MultiplicativeGaussianNoiseImageFilter() {}

private:
  MultiplicativeGaussianNoiseImageFilter(const Self &); //purposely not implemented
  void operator=(const Self &);     //purposely not implemented
};

} // End namespace itk

#endif /* __itkMultiplicativeGaussianNoiseImageFilter */
